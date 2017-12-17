#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QMap>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

#include <memory>
#include "tcpclient.h"
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    virtual ~TcpServer();
    void post_message(QString &session, QByteArray& data);
    void post_message(QByteArray& data);
    void start(const QString& host, int port);
    void stop();
    bool is_running();
signals:
    void on_stop();
    void on_message(QString& session, QByteArray& data);

public slots:
    void on_error(QAbstractSocket::SocketError err);
    void on_connect();
    void on_disconnected();
    void on_read();

private:
    bool running_{false};
    QTcpServer* server_;
    QMap<QString, QTcpSocket*> sessions_;
};

#endif // TCPSERVER_H
