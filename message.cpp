#include "message.h"

Message::Message()
{
    code = 400;
    date = QDateTime::currentDateTime();
    contentType = "text/html";
    contentLength = 0;
    server = "Vovach";
    keepAlive = true;
    messageFormed = false;
    end = false;
    strbody = 0;
    body = 0;
    header = new QBuffer();
}

QByteArray Message::getNextBlock(quint32 size)
{
    if (!messageFormed) {
        formMessage();
    }
    if (end) return "";
    QByteArray res;
    if (header->atEnd()) {
        if (body != 0)
            res = body->read(size);
        else
            end = true;
    } else {
        res = header->read(size);
        if (res.size() < size && body != 0)
            res.append(body->read(size - res.size()));
    }
    if(header->atEnd()) {
        if (body == 0)
            end = true;
        else if (body->atEnd())
            end = true;
    }
    return res;
}

void Message::formMessage()
{
    header->open(QIODevice::WriteOnly);
    QTextStream hs(header);
    hs << "HTTP/1.1 ";
    switch (code) {
    case 404: hs << "404 Not Found\r\n"; break;
    case 400: hs << "400 Bad Request\r\n"; break;
    case 403: hs << "403 Forbidden\r\n"; break;
    case 200: default: hs << "200 OK\r\n";    
    }
    hs << "Date: " << date.toString(Qt::RFC2822Date) << "\r\n";
    hs << "Server: " << server << "\r\n";
    if (!keepAlive)
        hs << "Connection: close\r\n";
    hs << "Content-Length: " << QByteArray::number(contentLength) << "\r\n";
    hs << "Content-Type: " << contentType << "\r\n";
    hs << "\r\n";
    header->close();
    messageFormed = true;
    header->open(QIODevice::ReadOnly);
    if (body != 0)
        body->open(QIODevice::ReadOnly);
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

bool Message::setKeepAlive(bool keepAlive)
{
    if (messageFormed) return false;
    this->keepAlive = keepAlive;
    return true;
}

bool Message::setBody(QIODevice *body)
{
    if (messageFormed) return false;
    this->body = body;
    return true;
}

bool Message::setBody(QByteArray *strbody)
{
    if (messageFormed) return false;
    this->strbody = strbody;
    this->body = new QBuffer(strbody);
    return true;
}

bool Message::endOfMessage()
{
    return end;
}

Message::~Message()
{
    if (header) delete header;
    if (body) delete body;
    if (strbody) delete strbody;
}
