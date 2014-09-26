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

void HttpClient::setKeepAlive(bool keepAlive)
{
    this->socketKeepAlive = socketKeepAlive;

    if (keepAlive) {
        inativityTimer.setSingleShot(true);

        connect(&inativityTimer, SIGNAL(timeout()), this, SLOT(onInativityTimerTimeout()));
    } else {
        disconnect(&inativityTimer, SIGNAL(timeout()), this, SLOT(onInativityTimerTimeout()));
    }
}

void HttpClient::setKeepAliveTimeout(int msec)
{
    inativityTimer.setInterval(msec);
}

void HttpClient::setClientActive()
{
    if (socketKeepAlive) {
        inativityTimer.start();
    }
}

void HttpClient::onBytesWritten()
{
    if (!message->endOfMessage()) {
        QByteArray a = message->getNextBlock(1024);

        socket->write(a);
    } else {
        if (!socketKeepAlive) {
            socket->disconnectFromHost();
        }
    }

    setClientActive();
}

void HttpClient::onReadyRead()
{
    QByteArray *request = new QByteArray();
    QBuffer buffer(request);

    buffer.open(QIODevice::ReadWrite);

    while (socket->isReadable() && !socket->atEnd()) {
       buffer.write(socket->readAll());
    }

    message = logic->handleRequest(request, socketKeepAlive);

    delete request;

    setClientActive();
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

void HttpClient::onInativityTimerTimeout()
{
    qDebug() << "Client Disconnecting due to inactivity";
    socket->disconnectFromHost();
}
