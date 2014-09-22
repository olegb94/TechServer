#include "cachecontrol.h"
#include <QFile>
#include <QBuffer>

CacheControl::CacheControl(QString root)
{
    this->root = root;
    maxCachedFileSize = 100;
    maxTotalCacheSize = 200;
    totalCacheSize = 0;
}


bool CacheControl::cacheFile(QString &path)
{
    QFile file(root+"."+path);
    if (!file.exists()) return false;
    quint64 fsize = file.size();
    if (fsize > maxCachedFileSize || (fsize+totalCacheSize > maxTotalCacheSize))
        return false;
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QByteArray *cache = new QByteArray(file.readAll());
    cachedFiles.insert(path, cache);
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

bool CacheControl::isFileExists(QString &path)
{
    QFile *file = new QFile(root + "." + path);
    bool isExist = file->exists();
    delete file;
    return isExist;
}
