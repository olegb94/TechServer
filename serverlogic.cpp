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
        response->setContentType(parseContentType(&uri));
        response->setBody(mesBody);
        response->setConnection(false);
        return response;
    }
    else if(method == "HEAD") {
        uri = uri.split("?")[0];
        if(!cacheControl.isFileExists(uri)) {
            return formBadRequestMessage();
        }
        Message *response = new Message();
        response->setCode(200);
        response->setContentLength(0);
        response->setContentType(parseContentType(&uri));
        response->setBody(0);
        response->setConnection(false);
        return response;
    }
    return formNotFoundMessage();
}

Message *ServerLogic::formNotFoundMessage() {
    Message *response = new Message();
    QByteArray *body = new QByteArray("404 Not Found");
    response->setCode(404);
    response->setContentLength(body->length());
    response->setContentType("text/plain");
    response->setBody(body);
    response->setConnection(false);
    return response;
}

Message *ServerLogic::formBadRequestMessage() {
    Message *response = new Message();
    QByteArray *body = new QByteArray("400 Bad Request");
    response->setCode(400);
    response->setContentLength(body->length());
    response->setContentType("text/plain");
    response->setBody(body);
    response->setConnection(false);
    return response;
}

QString ServerLogic::parseContentType(QString *uri)
{
    QString cType;
    QStringList cTypeToInt;
    cTypeToInt << "html"<< "css" << "js" << "jpg" << "jpeg" << "png" << "gif" << "swf" << "txt";
    QString cTypeMarker = uri->split('.').last();
    if(cTypeMarker == "html") {
        cType = "text/html";
    }
    else if(cTypeMarker == "css") {
        cType = "text/css";
    }
    else if(cTypeMarker == "js") {
        cType = "application/javascript";
    }
    else if(cTypeMarker == "jpg") {
        cType = "image/jpeg";
    }
    else if(cTypeMarker == "jpeg") {
        cType = "image/jpeg";
    }
    else if(cTypeMarker == "png") {
        cType = "image/png";
    }
    else if(cTypeMarker == "gif") {
        cType = "image/gif";
    }
    else if(cTypeMarker == "swf") {
        cType = "application/x-shockwave-flash";
    }
    else if(cTypeMarker == "txt") {
        cType = "text/html";
    }
    else cType = "text/html";
    return cType;
}

