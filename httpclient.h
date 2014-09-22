#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QBuffer>
#include "message.h"
#include "serverlogic.h"

class HttpClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    Message     *message;
    ServerLogic *logic;
public:
    HttpClient(QTcpSocket *socket, ServerLogic *logic);
    ~HttpClient();
signals:
    void    disconnected(HttpClient *httpClient);
public slots:
    void    onBytesWritten();
    void    onReadyRead();
    void    onError(QAbstractSocket::SocketError error);
    void    onDisconnected();
};

#endif // CONNECTION_H
