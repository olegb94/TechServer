#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <stdlib.h>
#include <Qt>
#include <QDebug>

class Message
{
public:
    Message();
    void formMessage();
    // true - success
    bool setCode(quint16 code);
    bool setDate(QDateTime date);
    bool setContentType(QString contentType);
    bool setContentLength(quint32 length);
    bool setServer(QString server);
    bool setConnection(bool keepAlive);
    bool setBody(QByteArray *body);

    QByteArray getNextBlock(quint32 size);
    bool endOfMessage();
private:
    quint16 code;
    QDateTime date;
    QString contentType;
    quint32 contentLength;
    QString server;
    bool keepAlive;
    bool messageFormed;
    QByteArray header;
    QByteArray *body;
    qint64 pos;
};


#endif // MESSAGE_H
