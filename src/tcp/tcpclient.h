#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include "aeio.h"
#define BUFFER_SIZE (1024 * 8)

class TcpClient: public QObject
{
        Q_OBJECT

public:
    explicit TcpClient(const char* addr, int port, QObject* parent = 0);
    TcpClient(const char *addr, int port, int fd, QObject* parent = 0);
    int get_fd();
    bool open(const char *addr = NULL, int port = 0);
    void close();
    int write(char *data, size_t len);

signals:
    void onmessage(int fd, char* data, unsigned int len);
protected:
    char neterr[ANET_ERR_LEN]{0x00};

private:
    int _fd;
    int _port;
    std::string _addr;
     char _buffer[BUFFER_SIZE + 1];

    void onread(aeEventLoop* loop, int fd, int mask);
};


#endif // TCPCLIENT_H
