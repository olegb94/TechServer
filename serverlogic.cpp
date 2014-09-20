#include "serverlogic.h"

ServerLogic::ServerLogic()
{
    root = "";
}

Message *ServerLogic::handleRequest(QByteArray *req)
{
    QBuffer request(req);
    request.open(QIODevice::ReadOnly);
    QByteArray startingLine = request.readLine();
    QList<QByteArray> parts = startingLine.split(' ');
    QByteArray &method = parts[0];    
    QUrl qUrl(parts[1]);
    //QByteArray &httpv = parts[2];
    if (method == "GET") {
        QString uri = qUrl.toString().split('?')[0];
        QByteArray *mesBody = getFileFromCache(uri);
        if (mesBody == 0) {
            mesBody = cacheFile(uri);
            if (mesBody == 0) {
                return formNotFoundMessage();
            }
        }
        Message *response = new Message();
        response->setCode(200);
        response->setContentLength(mesBody->length());
        response->setContentType("text/html");
        response->setBody(new QBuffer(mesBody));
        response->setConnection(false);
        return response;
    }
    return formNotFoundMessage();
}

Message *ServerLogic::formNotFoundMessage() {
    Message *response = new Message();
    response->setCode(404);
    response->setContentLength(13);
    response->setContentType("text/plain");
    response->setBodyString(new QByteArray("404 Not Found"));
    response->setConnection(false);
    return response;
}

Message *ServerLogic::formBadRequestMessage() {
    Message *response = new Message();
    response->setCode(400);
    response->setContentLength(15);
    response->setContentType("text/plain");
    response->setBodyString(new QByteArray("400 Bad Request"));
    response->setConnection(false);
    return response;
}

void ServerLogic::setRoot(QString root)
{
    this->root = root;
}

QByteArray *ServerLogic::cacheFile(QString path)
{
    QFile file(root+"."+path);
    //qDebug() << root+"."+path;
    if (!file.exists()) return 0;
    if (!file.open(QIODevice::ReadOnly)) return 0;
    QByteArray *cache = new QByteArray();
    *cache = file.readAll();
    cachedFiles.insert(path, cache);
    return cache;
}

QByteArray *ServerLogic::getFileFromCache(QString path)
{
    return cachedFiles.take(path);
}

