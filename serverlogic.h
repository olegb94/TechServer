#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include <QHash>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QBuffer>
#include <QDebug>
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
};

#endif // SERVERLOGIC_H
