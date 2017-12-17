#include "logdef.h"
#include "tcpserver.h"
#include <QMapIterator>

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server_ = new QTcpServer();
    connect(server_, &QTcpServer::acceptError, this, &TcpServer::on_error);
    connect(server_, &QTcpServer::newConnection, this, &TcpServer::on_connect);
}

TcpServer::~TcpServer()
{
    if (server_) {
        stop();
       server_->deleteLater();
    }

}

void TcpServer::start(const QString &host, int port)
{
    QHostAddress haddr(host);
    if (!server_->listen(haddr, port )) {
        qDebug() << server_->errorString();
        emit on_stop();
    } else {
        running_ = true;
    }
}

void TcpServer::stop()
{
    running_ = false;
    server_->close();
    QMapIterator<QString, QTcpSocket*> i(sessions_);
    while (i.hasNext()) {
        auto s = i.next();
        qDebug() << s.key();
        s.value()->disconnectFromHost();
    }
    sessions_.clear();
    emit on_stop();
}

bool TcpServer::is_running()
{
   return running_;
}

void TcpServer::post_message(QString& session, QByteArray& data)
{
    if (sessions_.contains(session)) {
    QTcpSocket* socket = sessions_[session];
       if ( socket->write(data) != data.size()) {
          LOGE("write failed");
       }
    }
}

void TcpServer::post_message(QByteArray &data)
{
   QMapIterator<QString, QTcpSocket*> i(sessions_);
   while (i.hasNext()) {
      auto s = i.next();
       if ( s.value()->write(data) != data.size()) {
          LOGE("write failed");
       }
   }
}

void TcpServer::on_error(QAbstractSocket::SocketError err)
{
    LOGE( "SocketError:%d", err);
}

void TcpServer::on_connect()
{
    QTcpSocket* socket = server_->nextPendingConnection();
    if (socket) {
        LOGD("new connect");
       connect(socket, &QTcpSocket::readyRead, this, &TcpServer::on_read);
         connect(socket, &QAbstractSocket::disconnected, this, &TcpServer::on_disconnected);
        QString session = QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
         sessions_[session]= socket;
    }
}

void TcpServer::on_disconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
   QMapIterator<QString, QTcpSocket*> i(sessions_);
   while (i.hasNext()) {
      auto s = i.next();
       if ( s.value() != socket) {
            sessions_.remove(s.key());
            break;
       }
   }
   socket->deleteLater();
}

void TcpServer::on_read()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray ba = socket->readAll();
    if (!ba.isEmpty()) {
    QString session = QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    emit on_message(session, ba);
    }
}
