#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <serverlogic.h>

class HttpServer: public QObject
{
    Q_OBJECT
private:
    QTcpServer server;
    ServerLogic logic;
public slots:
    void onNewConnection();
public:
    HttpServer(int port, QString document_root);
};

#endif // TCPSERVER_H
