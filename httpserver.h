#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

class HttpServer: public QObject
{
    Q_OBJECT
private:
    QTcpServer server;
public slots:
    void onNewConnection();
public:
    explicit HttpServer(QObject *parent = 0);
};

#endif // TCPSERVER_H
