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

class ServerLogic
{
public:
    ServerLogic();
    Message *handleRequest(QByteArray *request);
    void    setRoot(QString root);
private:
    QHash<QString, QByteArray *> cachedFiles;
    QString root;

    QByteArray *cacheFile(QString path);
    inline QByteArray *getFileFromCache(QString path);
    Message *formNotFoundMessage();
    Message *formBadRequestMessage();
};

#endif // SERVERLOGIC_H
