#include "httpserverworker.h"

HttpServerWorker::HttpServerWorker(ServerLogic *logic)
{
    this->logic = logic;
}

void HttpServerWorker::serveClient(QTcpSocket *client)
{
    emit newClient(client);
}

void HttpServerWorker::run()
{
    connect(this, SIGNAL(newClient(QTcpSocket*)), this, SLOT(onNewClient(QTcpSocket*)));

    while (true) {
        while (clients.count()) {
            QTcpSocket *client = newClients.takeFirst();

            if (client) {
                HttpClient *httpClient = new HttpClient(client, logic);

                clients.append(client);
            }
        }

        QThread::sleep(10);
    }
}

void HttpServerWorker::onNewClient(QTcpSocket *client)
{
    newClients.append(client);
}
