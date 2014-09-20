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

    initWorkers();

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void HttpServer::initWorkers()
{
    for (int i = 0; i < QThread::idealThreadCount(); ++i) {
        HttpServerWorker *worker = new HttpServerWorker(&logic);

        worker->start();
        workers.append(worker);
    }
}

void HttpServer::onNewConnection() {
    if (!server.hasPendingConnections())
        return;

    QTcpSocket *client = server.nextPendingConnection();

    //workers.first()->serveClient(client);
    HttpClient *httpClient = new HttpClient(client, &logic);
}
