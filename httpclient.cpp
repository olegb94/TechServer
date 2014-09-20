#include "httpclient.h"
#include <QBuffer>

HttpClient::HttpClient(QTcpSocket *socket, ServerLogic *logic)
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
    //qDebug() << "Socket killed\n";
}

void HttpClient::onBytesWritten()
{
    if (!message->endOfMessage()) {
        QByteArray a = message->getNextBlock(100);
        socket->write(a);
        //qDebug() << a;
    } else {
        socket->close();
        deleteLater();
    }
}

void HttpClient::onReadyRead()
{
    QByteArray *request = new QByteArray();
    QBuffer buffer(request);
    buffer.open(QIODevice::ReadWrite);
    while (socket->isReadable() && !socket->atEnd()) {
        //qDebug() << socket->readAll();
       buffer.write(socket->readAll());
    }
    message = logic->handleRequest(request);
    if (!message->endOfMessage()) {
        QByteArray a = message->getNextBlock(100);
        socket->write(a);
    }
}

void HttpClient::onError()
{
    qDebug() << "Error";
    this->deleteLater();
}

void HttpClient::onDisconnected()
{
    this->deleteLater();
}
