#ifndef CACHECONTROL_H
#define CACHECONTROL_H

#include <QHash>
#include <QIODevice>
#include <QSettings>

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
    CacheControl(QSettings *settings);
    QIODevice *getFile(QString &path);
    bool isFileExist(QString &path);
};

#endif // CACHECONTROL_H
