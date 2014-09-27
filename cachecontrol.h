#ifndef CACHECONTROL_H
#define CACHECONTROL_H

#include <QHash>
#include <QSet>
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
    QSet<QString> uncachableFiles;

    bool cacheFile(QString &path);
    bool isFileAvailiableInCache(QString &path);
    bool isFileAvailiableOnDisk(QString &path);
    bool isFileUncacheable(QString &path);
    void markFileAsUncacheable(QString &path);
    QByteArray *getFileFromCache(QString &path);
public:
    CacheControl(QSettings *settings);
    QIODevice *getFile(QString &path);
    bool isFileAvailiable(QString &path);
};

#endif // CACHECONTROL_H
