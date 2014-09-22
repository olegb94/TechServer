#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include <QHash>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QUrl>
#include "message.h"
#include <cachecontrol.h>

class ServerLogic
{
public:
    ServerLogic(QString root);
    Message *handleRequest(QByteArray *request);
private:
    QString root;
    CacheControl cacheControl;

    Message *formNotFoundMessage();
    Message *formBadRequestMessage();
    QString parseContentType(QString uri);
    bool uriSecCheck(QString uri);
};

#endif // SERVERLOGIC_H
