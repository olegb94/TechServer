#include "httpserver.h"

HttpServer::HttpServer(int port, QString document_root): logic(document_root)
{
    this->port = port;

    qsrand(QTime::currentTime().msec());

    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

HttpServer::~HttpServer()
{ }

bool HttpServer::start()
{
    if (!server.listen(QHostAddress::Any, port)) {
        std::cout << "Port " << port << " is already in use" << std::endl;
        return false;
    }

    initWorkers();

    return true;
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

        client->close();
        client->deleteLater();

        return;
    }

    client->setParent(NULL);
    client->moveToThread(worker->getThread());

    worker->serveClient(client);
}
