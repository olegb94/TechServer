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
    QString  uri= QUrl::fromPercentEncoding(parts[1]);
    if (method == "GET") {
        uri = uri.split('?')[0];
        QIODevice *mesBody = cacheControl.getFile(uri);
        if (mesBody == 0) {
             return formNotFoundMessage();
        }
        Message *response = new Message();
        response->setCode(200);
        response->setContentLength(mesBody->size());
        response->setContentType(parseContentType(uri));
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

QString ServerLogic::parseContentType(QString uri)
{
    QString cType;
    //enum cTypeToInt {html, css, js, jpg, jpeg, png, gif, swf, txt};
    QStringList cTypeToInt;
    cTypeToInt << "html"<< "css" << "js" << "jpg" << "jpeg" << "png" << "gif" << "swf" << "txt";
    qDebug() << cTypeToInt.indexOf(uri.split('.').last());
    switch (cTypeToInt.indexOf(uri.split('.').last())) {
    //case cTypeToint.indexOf("txt");
    case 0:
        cType = "text/html";
        break;
    case 1:
        cType = "text/css";
        break;
    case 2:
        cType = "application/javascript";
        break;
    case 3:
        cType = "image/jpg";
        break;
    case 4:
        cType = "image/jpeg";
        break;
    case 5:
        cType = "image/png";
        break;
    case 6:
        cType = "image/gif";
        break;
    case 7:
        cType = "application/x-shockwave-flash";
        break;
    case 8:
        cType = "text/html";
        break;
    default:
        cType = "text/html";
        break;
    }
    return cType;
}

