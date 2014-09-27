#include "serverlogic.h"


ServerLogic::ServerLogic(QSettings *settings)
{
    QString document_root = settings->value("server/document_root").toString();

    if (document_root.right(1) != "/") {
        document_root.push_back("/");
    }

    bool keepAliveAllowed = settings->value("server/keep_alive_allowed").toBool();

    this->root = document_root;
    this->cacheControl = new CacheControl(settings);
    this->keepAliveAllowed = keepAliveAllowed;
}

ServerLogic::~ServerLogic()
{
    delete cacheControl;
}

Message *ServerLogic::handleRequest(QByteArray *req, bool &socketKeepAlive)
{
    QBuffer request(req);

    request.open(QIODevice::ReadOnly);

    QByteArray startingLine = request.readLine();

    QByteArray method;
    QString uri;
    QHash<QString, QString> headers;

    parseStartingLine(startingLine, method, uri);
    parseHeaders(request, headers);

    socketKeepAlive = (keepAliveAllowed
                       && headers.contains("Connection")
                       && headers.value("Connection").toLower() == "keep-alive");

    if (!uriSecCheck(uri)) {
        return formBadRequestMessage(socketKeepAlive);
    }

    if(!checkForIndex(uri)) {
        return formForbiddenMessage(socketKeepAlive);
    }

    if (method != "GET" && method != "HEAD") {
        return formBadRequestMessage(socketKeepAlive);
    }

    QIODevice *mesBody = cacheControl->getFile(uri);

    if (mesBody == NULL) {
        return formNotFoundMessage(socketKeepAlive);
    }

    bool includeBody = (method == "GET");
    Message *message = formOKMessage(parseContentType(uri), socketKeepAlive, mesBody, includeBody);

    if (!includeBody) {
        delete mesBody;
    }

    return message;
}

Message *ServerLogic::formOKMessage(QString contentType, bool keepAlive, QIODevice *mesBody, bool includeBody) {
    Message *response = new Message();

    response->setCode(200);
    response->setContentLength(mesBody->size());
    response->setContentType(contentType);

    if (includeBody) {
        response->setBody(mesBody);
    }

    response->setKeepAlive(keepAlive);

    return response;
}

Message *ServerLogic::formNotFoundMessage(bool keepAlive) {
    Message *response = new Message();
    QByteArray *body = new QByteArray("404 Not Found");

    response->setCode(404);
    response->setContentLength(body->length());
    response->setContentType("text/plain");
    response->setBody(body);
    response->setKeepAlive(keepAlive);

    return response;
}

Message *ServerLogic::formBadRequestMessage(bool keepAlive) {
    Message *response = new Message();
    QByteArray *body = new QByteArray("400 Bad Request");

    response->setCode(400);
    response->setContentLength(body->length());
    response->setContentType("text/plain");
    response->setBody(body);
    response->setKeepAlive(keepAlive);

    return response;
}

Message *ServerLogic::formForbiddenMessage(bool keepAlive)
{
    Message *response = new Message();
    QByteArray *body = new QByteArray("403 Forbidden directory");

    response->setCode(403);
    response->setContentLength(body->length());
    response->setContentType("text/plain");
    response->setBody(body);
    response->setKeepAlive(keepAlive);

    return response;
}

void ServerLogic::parseStartingLine(QByteArray &startingLine, QByteArray &method, QString &uri)
{
    QList<QByteArray> startingLineParts = startingLine.split(' ');

    method = startingLineParts[0];

    int uriQuestionMarkIndex = startingLineParts[1].indexOf('?');

    QByteArray uriRaw = (uriQuestionMarkIndex != -1)
            ? startingLineParts[1].left(uriQuestionMarkIndex)
            : startingLineParts[1];

    uri = QUrl::fromPercentEncoding(uriRaw);
}

void ServerLogic::parseHeaders(QBuffer &request, QHash<QString, QString> &headers)
{
    while (!request.atEnd()) {
        QByteArray line = request.readLine();

        int colonIndex = line.indexOf(':');

        if (colonIndex == -1) {
            continue;
        }

        QString headerName = line.left(colonIndex);

        if (headerName != "Connection") {
            continue;
        }

        QString headerValue = line.right(line.length() - colonIndex - 1).trimmed();

        headers.insert(headerName, headerValue);
    }
}

QString ServerLogic::parseContentType(QString uri)
{
    QString cType;
    QString cTypeMarker = uri.split('.').last().toLower();
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
    else if(cTypeMarker == "ico") {
        cType = "image/vnd.microsoft.icon";
    }
    else cType = "text/html";
    return cType;
}

bool ServerLogic::uriSecCheck(QString uri) {
    if (uri[0] != '/')
        return false;
    // 0 - met nothing suspicious, 1 - met '/', 2 - met '/.', 3 - met '/..'
    quint8 state = 1;
    // >=0 - secure, <0 - unsecure
    qint8 seclevel = 0;
    quint32 size = uri.size();

    for (quint32 i = 1; i < size; ++i) {
        if (uri[i] == '/') {
            switch (state) {
            case 1: return false;
            case 3: --seclevel;
            }
            state = 1;
        } else
            if (uri[i] == '.') {
            switch (state) {
            case 1: state = 2; break;
            case 2: state = 3; break;
            case 3: return false;
            }
        } else {
            switch (state) {
            case 1: ++seclevel; state = 0; break;
            case 2: case 3: return false;
            }
        }

        if (seclevel < 0)
            return false;
    }
    return true;
}

bool ServerLogic::checkForIndex(QString &uri){
    if(uri.at(uri.length() - 1) == '/') {
        if(!cacheControl->isFileAvailiable(uri.append("index.html"))){
            return false;
        }
    }
    return true;
}

