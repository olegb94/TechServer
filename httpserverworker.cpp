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
        if (clients.count() > 0) {
            QTcpSocket *client = clients.takeFirst();

            if (client) {
                ServerLogic *logic = new ServerLogic();
                logic->setRoot("/Users/f1nal/");
                HttpClient *httpClient = new HttpClient(client, logic);
            }
        }

        QThread::sleep(10);
    }
}

void HttpServerWorker::onNewClient(QTcpSocket *client)
{
    clients.append(client);
}
