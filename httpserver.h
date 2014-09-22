#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QCoreApplication>
#include <QThread>
#include <iostream>
#include "httpserverworker.h"
#include "serverlogic.h"
#include "httpclient.h"

class HttpServer: public QObject
{
    Q_OBJECT
private:
    QThread *thread;
    int port;
    QTcpServer server;
    ServerLogic logic;
    QList<HttpServerWorker*> workers;
    void initWorkers();
    HttpServerWorker *getWorkerForClient();
public slots:
    void onNewConnection();
public:
    HttpServer(int port, QString document_root);
};

#endif // TCPSERVER_H
