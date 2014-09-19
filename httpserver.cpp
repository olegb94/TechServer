#include "httpserver.h"
#include "httpclient.h"
#include <QCoreApplication>

HttpServer::HttpServer(int port, QString document_root)
{
    if (!server.listen(QHostAddress::Any, port)) {
        qDebug() <<  "Server failed to start on port" << port;
        exit(1);
    }
    logic.setRoot(document_root);
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void HttpServer::onNewConnection() {
    if (!server.hasPendingConnections())
        return;
    HttpClient *client = new HttpClient(server.nextPendingConnection(), &logic, this);
}
