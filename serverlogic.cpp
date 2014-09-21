#include "serverlogic.h"


ServerLogic::ServerLogic(QString root):  root(root), cacheControl(root)
{

}


Message *ServerLogic::handleRequest(QByteArray *req)
{
    QBuffer request(req);
    request.open(QIODevice::ReadOnly);
    QByteArray startingLine = request.readLine();
    QList<QByteArray> parts = startingLine.split(' ');
    QByteArray &method = parts[0];
    QString  qUrl= QUrl::fromPercentEncoding(parts[1]);
    if (method == "GET") {
        QString uri = qUrl.split('?')[0];
        QIODevice *mesBody = cacheControl.getFile(uri);
        if (mesBody == 0) {
             return formNotFoundMessage();
        }
        Message *response = new Message();
        response->setCode(200);
        response->setContentLength(mesBody->size());
        response->setContentType("text/html");
        response->setBody(mesBody);
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
    response->setBody(new QByteArray("404 Not Found"));
    response->setConnection(false);
    return response;
}

Message *ServerLogic::formBadRequestMessage() {
    Message *response = new Message();
    response->setCode(400);
    response->setContentLength(15);
    response->setContentType("text/plain");
    response->setBody(new QByteArray("400 Bad Request"));
    response->setConnection(false);
    return response;
}

