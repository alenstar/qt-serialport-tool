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
  anetNonBlock(NULL, fd);
  anetEnableTcpNoDelay(NULL, fd);
  // anetKeepAlive(NULL, fd, server.tcpkeepalive);
  if (aeCreateFileEvent(
          loop, fd, AE_READABLE,
          [](aeEventLoop *loop, int fd, void *privdata, int mask) {
            TcpServer *thiz = static_cast<TcpServer *>(privdata);
            thiz->onread(loop, fd, mask);
          },
          NULL) == AE_ERR) {
    ::close(fd);
    LOGE("not create file event");
    return;
  } else {
    std::map<int, ClientInfo *>::iterator it = _clients.find(fd);
    if (it != _clients.end()) {
      _clients.erase(it, _clients.begin());
       unlinkFileEvent(loop, fd);
      delete it->second;
    }
    auto c = new ClientInfo;
    c->fd = fd;
    c->src_port = cport;
    strcpy(c->src_addr, cip);
    c->src_addr[strlen(cip)] = '\0';
    _clients.insert(std::make_pair(fd, c));
  }
  // write(fd, "hello world !", 14);
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
      LOGE("Reading from client: %s", strerror(errno));
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
  }
  }while(nread == BUFFER_SIZE);
  LOGD("onread: %s", _buffer);
}
