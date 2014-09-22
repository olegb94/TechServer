#include "httpclient.h"

HttpClient::HttpClient(QTcpSocket *socket, ServerLogic *logic) :
    socket(socket), logic(logic), message(NULL)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();

    qint64 bytes = socket->bytesAvailable();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    if (bytes) {
        emit socket->readyRead();
    }
}

HttpClient::~HttpClient()
{
    if (socket) {
        delete socket;
    }

    if (message) {
        delete message;
    }

    qDebug() << "Socket killed";
}

void HttpClient::onBytesWritten()
{
    if (!message->endOfMessage()) {
        QByteArray a = message->getNextBlock(1024);
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

    delete request;

    onBytesWritten();
}

void HttpClient::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Error: " << error;
}

void HttpClient::onDisconnected()
{
    qDebug() << "Client Disconnected";
    emit disconnected(this);
}
