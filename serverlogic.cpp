#include "serverlogic.h"
#include <QBuffer>
#include <QDebug>

ServerLogic::ServerLogic()
{
    root = "";
}

Message *ServerLogic::handleRequest(QByteArray *req)
{
    QBuffer request(req);
    request.open(QIODevice::ReadOnly);
    Message *response = new Message();
    QByteArray startingLine = request.readLine();
    QList<QByteArray> parts = startingLine.split(' ');
    if (parts[0] == "GET") {
        QByteArray *mesBody = getFileFromCash(parts[1]);
        if (mesBody == 0) {
            mesBody = cashFile(parts[1]);
            if (mesBody == 0) {
                return formNotFoundMessage();
            }
        }
        response->setCode(200);
        response->setContentLength(mesBody->length());
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
    response->setContentLength(0);
    response->setContentType("text/html");
    response->setBody(0);
    response->setConnection(false);
    return response;
}

Message *ServerLogic::formBadRequestMessage() {
    Message *response = new Message();
    response->setCode(400);
    response->setContentLength(0);
    response->setContentType("text/html");
    response->setBody(0);
    response->setConnection(false);
    return response;
}

void ServerLogic::setRoot(QString root)
{
    this->root = root;
}

QByteArray *ServerLogic::cashFile(QString path)
{
    QFile file(root+"."+path);
    //qDebug() << root+"."+path;
    if (!file.exists()) return 0;
    if (!file.open(QIODevice::ReadOnly)) return 0;
    QByteArray *cash = new QByteArray();
    *cash = file.readAll();
    cashedFiles.insert(path, cash);
    return cash;
}

QByteArray *ServerLogic::getFileFromCash(QString path)
{
    return cashedFiles.take(path);
}

