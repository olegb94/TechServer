#include "httpserverworker.h"

HttpServerWorker::HttpServerWorker(ServerLogic *logic, QSettings *settings)
{
    bool keepAliveAllowed = settings->value("server/keep_alive_allowed").toBool();
    int keepAliveTimeout = settings->value("server/keep_alive_timeout").toInt();

    this->logic = logic;
    this->keepAliveAllowed = keepAliveAllowed;
    this->keepAliveTimeout = keepAliveTimeout;

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

    connect(httpClient, SIGNAL(disconnected(HttpClient*)), this, SLOT(onHttpClientDisconnected(HttpClient*)));

    clients.append(httpClient);
}

void HttpServerWorker::onHttpClientDisconnected(HttpClient *httpClient)
{
    disconnect(httpClient, SIGNAL(disconnected(HttpClient*)), this, SLOT(onHttpClientDisconnected(HttpClient*)));

    clients.removeAll(httpClient);

    httpClient->deleteLater();
}
