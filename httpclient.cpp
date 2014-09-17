#include "httpclient.h"

HttpClient::HttpClient(QTcpSocket *socket, QObject *parent) :
    QObject(parent), socket(socket)
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

HttpClient::~HttpClient()
{
    delete socket;
    delete message;
    qDebug() << "Socket killed\n";
}

void HttpClient::onBytesWritten()
{
    if (!message->endOfMessage()) {
        QByteArray a = message->getNextBlock(19);
        socket->write(a);
        qDebug() << a;
    }
    else
        deleteLater();
}

void HttpClient::onReadyRead()
{
    while(socket->isReadable() && !socket->atEnd())
        qDebug() << socket->readAll();
    message = new Message();
    if (!message->endOfMessage()) {
        QByteArray a = message->getNextBlock(10);
        socket->write(a);
    }
}

void HttpClient::onError()
{
    this->deleteLater();
}

void HttpClient::onDisconnected()
{
    this->deleteLater();
}
