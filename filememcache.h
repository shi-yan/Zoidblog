#ifndef FILEMEMCACHE_H
#define FILEMEMCACHE_H

#include <QObject>
#include <QMap>
#include <QMutex>

#include "filememcachedata.h"

class FileMemCache:public QObject
{
    Q_OBJECT

    QMap<QString,FileMemCacheData> cachedFiles;

    QMutex cacheMutex;

    int maximumFileCount;

    int readerCount;

    bool needToAddNewFile;

    FileMemCache();
    ~FileMemCache();
public:

    static FileMemCache & getSingleton()
    {
        static FileMemCache obj;
        return obj;
    }

    void addFileToCache(const QString &filename,unsigned char *buffer,size_t len);
    void readCachedFile(const QString &filename,unsigned char **buffer,size_t *len);

};

#endif // FILEMEMCACHE_H
