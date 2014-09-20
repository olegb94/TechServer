#ifndef MESSAGE_H
#define MESSAGE_H
#include <QDateTime>
#include <QBuffer>

class Message
{
public:
    Message();
    virtual ~Message();

    // true - success
    bool setCode(quint16 code);
    bool setDate(QDateTime date);
    bool setContentType(QString contentType);
    bool setContentLength(quint32 length);
    bool setServer(QString server);
    bool setConnection(bool keepAlive);
    bool setBody(QIODevice *body);      //use if you need want to transmit file or cached file
    bool setBodyString(QByteArray *strbody);  //use if you need a strbody to be deleted after the message deleted

    void        formMessage();
    QByteArray  getNextBlock(quint32 size);
    bool        endOfMessage();

private:
    quint16     code;
    QDateTime   date;
    QString     contentType;
    quint32     contentLength;
    QString     server;
    bool        keepAlive;
    bool        messageFormed;  //if true it is impossible to set fields
    bool        end;
    QBuffer     *header;
    QIODevice   *body;
    QByteArray  *strbody;
};


#endif // MESSAGE_H
