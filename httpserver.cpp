#include "httpserver.h"

HttpServer::HttpServer(int port, QString document_root): logic(document_root)
{
    if (!server.listen(QHostAddress::Any, port)) {
        std::cout << "Server failed to start on port " << port << std::endl;
        exit(1);
    }

    initWorkers();

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void HttpServer::initWorkers()
{
    for (int i = 0; i < QThread::idealThreadCount(); ++i) {
        QThread *thread = new QThread();
        HttpServerWorker *worker = new HttpServerWorker(&logic);

        thread->start();
        worker->moveToThread(thread);

        workers.append(worker);
    }
}

void HttpServer::onNewConnection() {
    if (!server.hasPendingConnections())
        return;

    QTcpSocket *client = server.nextPendingConnection();

    workers.first()->serveClient(client);
    //HttpClient *httpClient = new HttpClient(client, &logic);
}
