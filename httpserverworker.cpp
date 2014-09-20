#include "httpserverworker.h"

HttpServerWorker::HttpServerWorker(ServerLogic *logic)
{
    this->logic = logic;

    connect(this, SIGNAL(newClient(QTcpSocket*)), this, SLOT(onNewClient(QTcpSocket*)));
}

void HttpServerWorker::serveClient(QTcpSocket *client)
{
    emit newClient(client);
}

void HttpServerWorker::setThread(QThread *thread)
{
    this->thread = thread;
}

QThread *HttpServerWorker::getThread()
{
    return thread;
}

void HttpServerWorker::onNewClient(QTcpSocket *client)
{
    if (!client) {
        return;
    }

    HttpClient *httpClient = new HttpClient(client, logic);

    clients.append(httpClient);
}
