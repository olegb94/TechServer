#include "httpclient.h"

HttpClient::HttpClient(QTcpSocket *socket, QObject *parent) :
    QObject(parent), socket(socket)
{
    //this->socket = socket;
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
    socket->write("Content-Length: 2\r\n");
    socket->write("\r\n");
    socket->write("lol");
}

void HttpClient::onReadyRead()
{
    while(socket->isReadable() && !socket->atEnd())
        qDebug() << socket->readAll();
    socket->write("HTTP/1.1 200 OK\r\n");
    socket->write("Date: Thu, 19 Feb 2009 12:27:04 GMT\r\n");
    socket->write("Content-Type: text/html; charset=windows-1251\r\n");

}

void HttpClient::onError()
{
    this->deleteLater();
}

void HttpClient::onDisconnected()
{
    this->deleteLater();
}
