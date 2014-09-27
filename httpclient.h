#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QBuffer>
#include <QTimer>
#include "message.h"
#include "serverlogic.h"

class HttpClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket  *socket;
    Message     *message;
    ServerLogic *logic;
    bool        socketKeepAlive;
    QTimer      inativityTimer;
    void        setClientActive();
public:
    HttpClient(QTcpSocket *socket, ServerLogic *logic);
    ~HttpClient();
    void setKeepAlive(bool keepAlive);
    void setKeepAliveTimeout(int msec);
signals:
    void    disconnected(HttpClient *httpClient);
private slots:
    void    onBytesWritten();
    void    onReadyRead();
    void    onError(QAbstractSocket::SocketError error);
    void    onDisconnected();
    void    onInativityTimerTimeout();
};

#endif // CONNECTION_H
