#ifndef HTTPSERVERWORKER_H
#define HTTPSERVERWORKER_H

#include <QTcpSocket>
#include <QThread>
#include <httpclient.h>

class HttpServerWorker : public QThread
{
    Q_OBJECT
public:
    HttpServerWorker(ServerLogic *logic);
    void serveClient(QTcpSocket *client);
protected:
    void run();
private:
    ServerLogic *logic;
    QList<QTcpSocket*> clients;
signals:
    void newClient(QTcpSocket *client);
public slots:
    void onNewClient(QTcpSocket *client);
};

#endif // HTTPSERVERWORKER_H
