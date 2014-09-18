#include "httpserver.h"
#include "httpclient.h"
#include <QCoreApplication>
#define DOCUMENT_ROOT "/Users/oleg/www/"

HttpServer::HttpServer(QObject *parent): QObject(parent)
{
    if (!server.listen(QHostAddress::Any, 8001)) {
        qDebug() <<  "Server failed to start\n";
        exit(1);
    }
    logic.setRoot(DOCUMENT_ROOT);
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void HttpServer::onNewConnection() {
    if (!server.hasPendingConnections())
        return;
    HttpClient *client = new HttpClient(server.nextPendingConnection(), &logic, this);
}
