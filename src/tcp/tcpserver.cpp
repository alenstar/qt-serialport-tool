#include "tcpserver.h"
#include "aeio.h"
#include "logdef.h"
#include "loop/loop.h"
#include "tcpclient.h"
#include <cstdio>
#include <cstdlib>
#define MAX_ACCEPTS_PER_CALL 1000
/* Anti-warning macro... */
#define UNUSED(V) ((void)V)
#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */
#define NET_PEER_ID_LEN (NET_IP_STR_LEN + 32) /* Must be enough for ip:port */

TcpServer::TcpServer(const char *addr, int port, QObject *parent)
    : QObject(parent), _fd(0), _addr(addr), _port(port) {}

TcpServer::~TcpServer()
{

}

int TcpServer::get_fd() { return _fd; }

bool TcpServer::listen() {
  int sfd = anetTcpServer(neterr, _port, (char *)_addr.c_str(), 4);
  if (sfd != ANET_ERR) {
    anetNonBlock(NULL, sfd);
    if (aeCreateFileEvent(
            Loop::default_loop(), sfd, AE_READABLE,
            [](aeEventLoop *loop, int fd, void *privdata, int mask) {
              // UNUSED(loop);
              TcpServer *thiz = static_cast<TcpServer *>(privdata);
              thiz->onaccept(loop, fd, mask);
            },
            this)) {
      LOGE("Unrecoverable errror createing server file event");
      return false;
    } else {
      _fd = sfd;
    }
  } else if (errno == EAFNOSUPPORT) {
    LOGE("Not listening to IPv4: unsupported");
    return false;
  }
  return true;
}

void TcpServer::close() {
  unlinkFileEvent(Loop::default_loop(), _fd);
  _fd = 0;
}

bool TcpServer::is_running() { return _fd > 0 ? true : false; }

void TcpServer::set_parameters(const char *addr, int port) {
  _addr = std::string(addr);
  _port = port;
}

void TcpServer::onaccept(aeEventLoop *loop, int fd, int mask) {
  UNUSED(mask);
  int cport, cfd;
  char cip[NET_IP_STR_LEN];
  cfd = anetTcpAccept(neterr, fd, cip, sizeof(cip), &cport);
  if (cfd == ANET_ERR) {
    if (errno != EWOULDBLOCK)
      LOGE("Accepting client connection: %s", neterr);
    return;
  }

  LOGD("Accepted %s:%d", cip, cport);
  // acceptCommonHandler(loop, cfd, 0, cip);
  anetNonBlock(NULL, cfd);
  anetEnableTcpNoDelay(NULL, cfd);
  // anetKeepAlive(NULL, fd, server.tcpkeepalive);
  if (aeCreateFileEvent(
          loop, cfd, AE_READABLE,
          [](aeEventLoop *loop, int fd, void *privdata, int mask) {
            TcpServer *thiz = static_cast<TcpServer *>(privdata);
            thiz->onread(loop, fd, mask);
          },
          this) == AE_ERR) {
    ::close(cfd);
    LOGE("not create file event");
    return;
  } else {
    std::map<int, ClientInfo *>::iterator it = _clients.find(fd);
    if (it != _clients.end()) {
      _clients.erase(it, _clients.begin());
       unlinkFileEvent(loop, cfd);
      delete it->second;
    }
    auto c = new ClientInfo;
    c->fd = cfd;
    c->src_port = cport;
    strcpy(c->src_addr, cip);
    c->src_addr[strlen(cip)] = '\0';
    _clients.insert(std::make_pair(cfd, c));
  }
}

void TcpServer::onread(aeEventLoop *loop, int fd, int mask) {
  int nread = 0;
  UNUSED(mask);
  do{
  nread = ::read(fd, _buffer, BUFFER_SIZE);
  if (nread == -1) {
    if (errno == EAGAIN) {
        LOGD("buffer empty");
      return;
    } else {
      LOGE("Reading(%d %d) from client: %s",_fd, fd, strerror(errno));
      unlinkFileEvent(loop, fd);
      //_clients.remove(fd);
      std::map<int, ClientInfo *>::iterator it = _clients.find(fd);
      if (it != _clients.end()) {
        _clients.erase(it, _clients.begin());
        delete it->second;
      }
      return;
    }
  } else if (nread == 0) {
    LOGI("Client closed connection");
    unlinkFileEvent(loop, fd);
    return;
  } else {
      char peer[16 + 5] = {0x00};
      char sock[16 + 5] = {0x00};
      anetFormatPeer(fd, peer, 21);
      anetFormatSock(fd, sock, 21);
      _buffer[nread] = '\0';
    LOGD("onread[%3d]: %s --> %s %s", nread, peer, sock,  _buffer);
    /*
    if (this->onmessage) {
        this->onmessage(fd, _buffer, nread);
    }
    */
    emit this->onmessage(fd, _buffer, nread);
  }
  }while(nread == BUFFER_SIZE);
}
