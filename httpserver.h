#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QCoreApplication>
#include <QThread>
#include <QSettings>
#include <iostream>
#include "httpserverworker.h"
#include "serverlogic.h"
#include "httpclient.h"

class HttpServer: public QObject
{
    Q_OBJECT
private:
    QSettings *settings;
    QTcpServer server;
    ServerLogic *logic;
    QList<HttpServerWorker*> workers;
    void initWorkers();
    HttpServerWorker *getWorkerForClient();
public slots:
    void onNewConnection();
public:
    HttpServer(QSettings *settings);
    ~HttpServer();
    bool start();
};

#endif // TCPSERVER_H
