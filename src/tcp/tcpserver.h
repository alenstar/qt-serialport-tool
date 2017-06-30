#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <functional>
#include <QObject>
#include "aeio.h"

#define BUFFER_SIZE (1024 * 8)

class TcpClient;
typedef struct ClientInfo{
    int fd;
    int src_port;
    int dst_port;
    char src_addr[16];
    char dst_addr[16];
} ClientInfo;

class TcpServer:public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(const char* addr, int port, QObject* parent = 0);
    ~TcpServer();
    int get_fd();
    bool listen();
    void close();
    bool is_running();
    void set_parameters(const char* addr, int port);

    // std::function<void(int fd, char* data, size_t len)> onmessage;
signals:
    void onmessage(int fd, char* data, unsigned int len);

protected:
    char neterr[ANET_ERR_LEN]{0x00};

private:
    int _fd;
    int _port;
    std::string _addr;
private:
    void onaccept(aeEventLoop *loop, int fd, int mask);
    void onread(aeEventLoop* loop, int fd, int mask);

    //std::list<int> _clients;
    std::map<int, ClientInfo*> _clients;
    char _buffer[BUFFER_SIZE + 1];
};

#endif // TCPSERVER_H
