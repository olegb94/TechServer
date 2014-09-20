#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H
#include "message.h"
#include <QHash>
#include <QByteArray>
#include <QString>
#include <QFile>

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
