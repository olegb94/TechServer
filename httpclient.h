#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <message.h>
#include <QString>
#include <QTcpSocket>
#include <serverlogic.h>


class HttpClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;
    Message *message;
    ServerLogic *logic;
    virtual ~HttpClient();
public:
    HttpClient(QTcpSocket *socket, ServerLogic *logic);
signals:
public slots:
    void onBytesWritten();
    void onReadyRead();
    void onError();
    void onDisconnected();
};

#endif // CONNECTION_H
