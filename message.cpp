#include "message.h"
#include <stdlib.h>
#include <Qt>
#include <QDebug>

Message::Message()
{
    code = 200;
    date = QDateTime::currentDateTime();
    contentType = "text/html";
    contentLength = 59;
    server = "Vovach";
    keepAlive = true;
    messageFormed = false;
    header = "HTTP/1.1 ";
    pos = 0;
    body = new QByteArray("<html><title>test</title><body><h1>Hello!<h1></body></html>");
}

QByteArray Message::getNextBlock(quint32 size)
{
    if (!messageFormed) {
        formMessage();
        messageFormed = true;
    }
    quint32 h_size = header.size();
    QByteArray a;
    if (h_size > pos+size+1) {
        a = header.mid(pos, size);
        pos += size;
    } else if (body == 0) {
        a = header.right(h_size-pos-1);
        pos = -1;
    } else {
        quint32 b_size = body->size();
        if (h_size > pos) {
            a = header.right(h_size-pos-1) + body->left(pos+size+1-h_size);
            pos += size;
        } else if (h_size + b_size > pos+size+1) {
            a = body->mid(pos+1-h_size, size);
            pos += size;
        } else {
            a = body->right(h_size+b_size-pos-1);
            pos = -1;
        }
    }
    return a;
}

void Message::formMessage()
{
    switch (code) {
    case 404: header.append("404 Not Found\r\n"); break;
    case 400: header.append("400 Bad Request\r\n"); break;
    case 200: default: header.append("200 OK\r\n"); break;
    }
    header.append("Date: ").append(date.toString(Qt::RFC2822Date)).append("\r\n");
    header.append("Server: ").append(server).append("\r\n");
    if (!keepAlive)
        header.append("Connection: close\r\n");
    if (code == 200) {
        header.append("Content-Length: ").append(QByteArray::number(contentLength)).append("\r\n");
        header.append("Content-Type: ").append(contentType).append("\r\n");
    }
    header.append("\r\n");
    //qDebug() << header;
}

bool Message::setCode(quint16 code)
{
    if (messageFormed) return false;
    if (code == 200 || code == 400 || code == 404) {
        this->code = code;
        return true;
    }
    return false;
}

bool Message::setDate(QDateTime date)
{
    if (messageFormed) return false;
    this->date = date;
    return true;
}

bool Message::setContentType(QString contentType)
{
    if (messageFormed) return false;
    this->contentType = contentType;
    return true;
}

bool Message::setContentLength(quint32 length)
{
    if (messageFormed) return false;
    this->contentLength = length;
    return true;
}

bool Message::setServer(QString server)
{
    if (messageFormed) return false;
    this->server = server;
    return true;
}

bool Message::setConnection(bool keepAlive)
{
    if (messageFormed) return false;
    this->keepAlive = keepAlive;
    return true;
}

bool Message::setBody(QByteArray *body)
{
    if (messageFormed) return false;
    this->body = body;
    return true;
}

bool Message::endOfMessage()
{
    return (pos == -1) ? true : false;
}
