#include "logdef.h"
#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
 socket_ = new QTcpSocket(this);
 connect(socket_, &QTcpSocket::readyRead, this, &TcpClient::on_read);
 connect(socket_,  QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpClient::on_error);
 connect(socket_,  &QTcpSocket::connected, this, &TcpClient::on_connected);
}

TcpClient::~TcpClient()
{
   stop();
}

void TcpClient::stop()
{
running_ = false;
socket_->close();
}

void TcpClient::start(const QString& host, int port)
{
    session_ = QString("%1:%2").arg(host).arg(port);
    socket_->abort();
    socket_->connectToHost(host, port);
    running_ = true;
}

void TcpClient::post_message(QString &session, QByteArray &data)
{
    if (socket_->write(data) != data.size()) {
       LOGE("write error");
    }
}

void TcpClient::post_message(QByteArray &data)
{
    if (socket_->write(data) != data.size()) {
       LOGE("write error");
    }
}

bool TcpClient::is_running()
{
    return running_;
}

void TcpClient::on_connected()
{
    LOGD("connected");
}

void TcpClient::on_read()
{
    QByteArray ba = socket_->readAll();
    if (!ba.isEmpty()) {
       emit on_message(session_, ba);
    }
}

void TcpClient::on_error(QAbstractSocket::SocketError err)
{
    LOGE("SocketError:%d", err);
}
