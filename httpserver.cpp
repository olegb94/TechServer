#include "httpserver.h"
#include "httpclient.h"
#include <QCoreApplication>

HttpServer::HttpServer(QObject *parent): QObject(parent)
{
    if (!server.listen(QHostAddress::Any, 8001)) {
        qDebug() <<  "Server failed to start\n";
        QCoreApplication::quit();
    }

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void HttpServer::onNewConnection() {
    if (!server.hasPendingConnections())
        return;
    HttpClient *client = new HttpClient(server.nextPendingConnection(), this);
}
