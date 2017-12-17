#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    virtual ~TcpClient();
    void stop();
    void start(const QString &host, int port);
    void post_message(QString& session, QByteArray& data);
    void post_message(QByteArray& data);
    bool is_running();

signals:
    void on_message(QString& session, QByteArray& data);

public slots:
    void on_connected();
    void on_read();
    void on_error(QAbstractSocket::SocketError err);
private:
    bool running_{false};
    QString session_;
    QTcpSocket* socket_;
};

#endif // TCPCLIENT_H
