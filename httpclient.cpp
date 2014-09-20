#include "httpclient.h"

HttpClient::HttpClient(QTcpSocket *socket, ServerLogic *logic, QObject *parent) :
    QObject(parent), socket(socket), logic(logic)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
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
        QByteArray a = message->getNextBlock(100);
        //qDebug() << a;
        socket->write(a);
    } else {
        socket->close();
    }
}

void HttpClient::onReadyRead()
{
    QByteArray *request = new QByteArray();
    QBuffer buffer(request);
    buffer.open(QIODevice::ReadWrite);
    while (socket->isReadable() && !socket->atEnd()) {
       buffer.write(socket->readAll());
    }
    message = logic->handleRequest(request);
    onBytesWritten();
}

void HttpClient::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Error" << error;
}

void HttpClient::onDisconnected()
{
    qDebug() << "Client Disconnected";
}
