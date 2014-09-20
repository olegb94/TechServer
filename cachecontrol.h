#ifndef CACHECONTROL_H
#define CACHECONTROL_H

#include <QHash>
#include <QIODevice>

class CacheControl
{
private:
    quint64 totalCacheSize;
    quint64 maxTotalCacheSize;
    quint64 maxCachedFileSize;

    QString root;
    QHash<QString, QByteArray *> cachedFiles;
    bool cacheFile(QString &path);
    QByteArray *getFileFromCache(QString &path);
public:
    CacheControl(QString root);
    QIODevice *getFile(QString &path);
};

#endif // CACHECONTROL_H
