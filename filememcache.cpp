#include "filememcache.h"

FileMemCache::FileMemCache():QObject(),maximumFileCount(30),readerCount(0),needToAddNewFile(false)
{

}

FileMemCache::~FileMemCache()
{

}

void FileMemCache::addFileToCache(const QString &filename,unsigned char *buffer,size_t len)
{

}

void FileMemCache::readCachedFile(const QString &filename,unsigned char **buffer,size_t *len)
{
    cacheMutex.lock();

    while(needToAddNewFile)
    {
        cacheMutex.unlock();

        cacheMutex.lock();
    }
}
