#ifndef HTTPSERVERWORKER_H
#define HTTPSERVERWORKER_H

#include <QTcpSocket>
#include <QThread>
#include "httpclient.h"

class HttpServerWorker : public QObject
{
    Q_OBJECT
public:
    HttpServerWorker(ServerLogic *logic, QSettings *settings);
    void serveClient(QTcpSocket *client);
    void setThread(QThread *thread);
    QThread *getThread();
private:
    ServerLogic *logic;
    QList<HttpClient*> clients;
    QThread *thread;
    bool keepAliveAllowed;
    int keepAliveTimeout;
signals:
    void newClient(QTcpSocket *client);
private slots:
    void onNewClient(QTcpSocket *client);
    void onHttpClientDisconnected(HttpClient *httpClient);
};

#endif // HTTPSERVERWORKER_H
