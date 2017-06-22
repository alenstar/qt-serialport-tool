#include "tcpclient.h"

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

bool TcpClient::connect()
{
    return false;
}

void TcpClient::disconnect()
{

}

int TcpClient::write(char* data, size_t len)
{
    return anetWrite(_fd, data, len);
}

void TcpClient::onread(aeEventLoop *loop, int fd, int mask)
{

}

