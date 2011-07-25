#include "filememcachedata.h"
#include <cstdlib>

FileMemCacheData::FileMemCacheData():QObject(),filename(),buffer(0),bufferSize(0),lastAccessTime(QDateTime::currentDateTime())
{

}


FileMemCacheData::FileMemCacheData(const QString _filename,unsigned char *_buffer,size_t _bufferSize)
    :QObject(),filename(),buffer(0),bufferSize(0),lastAccessTime(QDateTime::currentDateTime())
{

}

FileMemCacheData::FileMemCacheData(const FileMemCacheData &in)
    :QObject(),filename(in.filename),buffer(in.buffer),bufferSize(in.bufferSize),
      lastAccessTime(in.lastAccessTime)
{
}

void FileMemCacheData::operator=(const FileMemCacheData &in)
{
    filename=in.filename;
    buffer=in.buffer;
    bufferSize=in.bufferSize;
    lastAccessTime=in.lastAccessTime;
}

void FileMemCacheData::clearBuffer()
{
    free(buffer);
    buffer=0;
    bufferSize=0;
}

void FileMemCacheData::updateLastAccessTime()
{
    lastAccessTime=QDateTime::currentDateTime();
}
