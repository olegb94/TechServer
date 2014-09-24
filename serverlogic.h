#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include <QHash>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QUrl>
#include <QSettings>
#include "message.h"
#include <cachecontrol.h>

class ServerLogic
{
public:
    ServerLogic(QSettings *settings);
    ~ServerLogic();
    Message *handleRequest(QByteArray *request, bool &socketKeepAlive);
private:
    QString root;
    CacheControl *cacheControl;
    Message *formOKMessage(QString contentType, bool keepAlive, QIODevice *mesBody, bool includeBody);
    Message *formNotFoundMessage(bool keepAlive);
    Message *formBadRequestMessage(bool keepAlive);
    void parseStartingLine(QByteArray &startingLine, QByteArray &method, QString &uri);
    void parseHeaders(QBuffer &request, QHash<QString, QString> &headers);
    QString parseContentType(QString uri);
    bool uriSecCheck(QString uri);
};

#endif // SERVERLOGIC_H
