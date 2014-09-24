#include "cachecontrol.h"
#include <QFile>
#include <QBuffer>

CacheControl::CacheControl(QSettings *settings)
{
    QString document_root = settings->value("server/document_root").toString();
    int maxCachedFileSize = settings->value("cache/max_cached_file_size").toInt();
    int maxTotalCacheSize = settings->value("cache/max_total_cache_size").toInt();

    if (document_root.right(1) != "/") {
        document_root.push_back("/");
    }

    this->root = document_root;
    this->maxCachedFileSize = maxCachedFileSize;
    this->maxTotalCacheSize = maxTotalCacheSize;
    this->totalCacheSize = 0;
}

bool CacheControl::cacheFile(QString &path)
{
    QFile file(root + "." + path);
    if (!file.exists()) return false;
    quint64 fsize = file.size();
    if (fsize > maxCachedFileSize || (fsize+totalCacheSize > maxTotalCacheSize))
        return false;
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QByteArray *cache = new QByteArray(file.readAll());
    cachedFiles.insert(path, cache);
    totalCacheSize += fsize;
    return true;
}


QByteArray *CacheControl::getFileFromCache(QString &path)
{
    return cachedFiles.value(path);
}


QIODevice *CacheControl::getFile(QString &path)
{
    QByteArray *strfile = getFileFromCache(path);
    if (strfile == 0) {
        if (cacheFile(path)) {
            strfile = getFileFromCache(path);
        } else {
            QFile *file = new QFile(root + "." + path);
            if (file->exists()) {
                return file;
            } else {
                delete file;
                return 0;
            }
        }
    }
    return new QBuffer(strfile);
}

bool CacheControl::isFileExist(QString &path)
{
    if(!cachedFiles.contains(path)) {
        QFile file(root + "." + path);
        if (!file.exists()) {
            return false;
        }
    }
    return true;
}
