#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include "aeio.h"

class TcpClient: public QObject
{
public:
    explicit TcpClient(const char* addr, int port, QObject* parent = 0);
    TcpClient(const char *addr, int port, int fd, QObject* parent = 0);
    int get_fd();
    bool connect();
    void disconnect();
    int write(char *data, size_t len);
    void onread(aeEventLoop* loop, int fd, int mask);
private:
    int _fd;
    int _port;
    std::string _addr;
};


#endif // TCPCLIENT_H
