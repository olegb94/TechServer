#include "cachecontrol.h"
#include <QFile>
#include <QFileInfo>
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
    if (isFileUncacheable(path))
        return false;

    if (!isFileAvailiableOnDisk(path)) {
        markFileAsUncacheable(path);
        return false;
    }

    QFile file(root + "." + path);
    quint64 fsize = file.size();

    if (fsize > maxCachedFileSize || (fsize+totalCacheSize > maxTotalCacheSize)) {
        markFileAsUncacheable(path);
        return false;
    }
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

    if (!isFileAvailiableInCache(path)) {
        if (!cacheFile(path)) {

            if (!isFileAvailiableOnDisk(path))
                return 0;

            return new QFile(root + "." + path);

        }
    }
    return new QBuffer(getFileFromCache(path));
}

bool CacheControl::isFileAvailiable(QString &path) {
    if (isFileAvailiableInCache(path))
        return true;

    if (isFileAvailiableOnDisk(path))
        return true;

    return false;
}

bool CacheControl::isFileAvailiableInCache(QString &path) {
    if (cachedFiles.contains(path))
        return true;
    return false;
}

bool CacheControl::isFileAvailiableOnDisk(QString &path) {
    QFileInfo file(root + "." +path);
    if (!file.exists() || file.isHidden() || file.isDir() || file.isSymLink() || !file.isReadable())
        return false;
    return true;
}

bool CacheControl::isFileUncacheable(QString &path) {
    return uncachableFiles.contains(path);
}

void CacheControl::markFileAsUncacheable(QString &path) {
    uncachableFiles += path;
}
