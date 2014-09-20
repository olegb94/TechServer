#include "httpserver.h"

HttpServer::HttpServer(int port, QString document_root)
{
    if (!server.listen(QHostAddress::Any, port)) {
        std::cout << "Server failed to start on port " << port << std::endl;
        exit(1);
    }

    qsrand(QTime::currentTime().msec());

    logic.setRoot(document_root);

    initWorkers();

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void HttpServer::initWorkers()
{
    for (int i = 0; i < QThread::idealThreadCount(); ++i) {
        QThread *thread = new QThread();
        HttpServerWorker *worker = new HttpServerWorker(&logic);

        thread->start();

        worker->setThread(thread);
        worker->moveToThread(thread);

        workers.append(worker);
    }
}

HttpServerWorker *HttpServer::getWorkerForClient()
{
    if (!workers.count()) {
        return NULL;
    }

    int workerIndex = qrand() % workers.count();

    qDebug() << "Serving incoming client with worker" << (workerIndex + 1);

    return workers.at(workerIndex);
}

void HttpServer::onNewConnection() {
    if (!server.hasPendingConnections())
        return;

    QTcpSocket *client = server.nextPendingConnection();
    HttpServerWorker *worker = getWorkerForClient();

    if (!worker) {
        std::cout << "Availiable worker not found" << std::endl;
    }

    client->setParent(NULL);
    client->moveToThread(worker->getThread());

    worker->serveClient(client);
}
