#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <message.h>
#include <QString>
#include <QTcpSocket>

class HttpClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;
    Message *message;
    virtual ~HttpClient();
public:
    explicit HttpClient(QTcpSocket *socket, QObject *parent = 0);
signals:
public slots:
    void onBytesWritten();
    void onReadyRead();
    void onError();
    void onDisconnected();
};

#endif // CONNECTION_H
