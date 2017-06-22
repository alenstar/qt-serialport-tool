#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QObject>
#include "aeio.h"
class TcpClient;
class TcpServer:public QObject
{
public:
    explicit TcpServer(const char* addr, int port, QObject* parent = 0);
    int get_fd();
    bool listen();
    void close();
    bool is_running();
    void set_parameters(const char* addr, int port);
    void onaccept(aeEventLoop *loop, int fd, int mask);
    void onread(aeEventLoop* loop, int fd, int mask);
protected:
    char neterr[ANET_ERR_LEN]{0x00};
private:
    int _fd;
    int _port;
    std::string _addr;
private:
    std::list<int> _clients;
};

#endif // TCPSERVER_H
