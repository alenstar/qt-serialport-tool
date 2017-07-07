#include "tcpclient.h"
#include "loop/loop.h"
#include "logdef.h"

#define UNUSED(V) ((void)V)
#define NET_IP_STR_LEN 46 /* INET6_ADDRSTRLEN is 46, but we need to be sure */
#define NET_PEER_ID_LEN (NET_IP_STR_LEN + 32) /* Must be enough for ip:port */

TcpClient::TcpClient(const char *addr, int port, QObject *parent):
    QObject(parent), _fd(0),_addr(addr), _port(port)
{

}

TcpClient::TcpClient(const char *addr, int port, int fd, QObject *parent) :
    QObject(parent),_fd(fd), _addr(addr), _port(port)
{
}

int TcpClient::get_fd()
{
    return _fd;
}

bool TcpClient::open(const char *addr, int port)
{
    if (_fd) {
        LOGD("connection is open");
        return true;
    }
    if (addr) {
    _addr = std::string(addr);
    }
    if (port){
      _port = port;
    }
    int fd = anetTcpConnect(neterr, (char*)_addr.c_str(), _port);
    if (fd != 0 && fd != ~0) {
        anetNonBlock(NULL, fd);
        anetEnableTcpNoDelay(NULL, fd);
        // anetKeepAlive(NULL, fd, server.tcpkeepalive);
        if (aeCreateFileEvent(
                Loop::default_loop(), fd, AE_READABLE,
                [](aeEventLoop *loop, int fd, void *privdata, int mask) {
                  TcpClient *thiz = static_cast<TcpClient*>(privdata);
                  thiz->onread(loop, fd, mask);
                },
                this) == AE_ERR) {
          ::close(fd);
          LOGE("not create file event");
          return false;
        } else {
            // TODO
            _fd = fd;
        }
        return true;
    }
    return false;
}

void TcpClient::close()
{

}

int TcpClient::write(char* data, size_t len)
{
    return anetWrite(_fd, data, len);
}

void TcpClient::onread(aeEventLoop *loop, int fd, int mask)
{
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
      _fd = 0;
      return;
    }
  } else if (nread == 0) {
    LOGI("Client closed connection");
    unlinkFileEvent(loop, fd);
    _fd = 0;
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
