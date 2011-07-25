#ifndef FILEMEMCACHEDATA_H
#define FILEMEMCACHEDATA_H

#include <QObject>
#include <QDateTime>

class FileMemCacheData:public QObject
{
    Q_OBJECT

    QString filename;
    unsigned char* buffer;
    size_t bufferSize;
    QDateTime lastAccessTime;

public:
    FileMemCacheData();
    FileMemCacheData(const QString _filename,unsigned char *_buffer,size_t _bufferSize);
    FileMemCacheData(const FileMemCacheData &in);
    void operator=(const FileMemCacheData &in);

    void clearBuffer();
    void updateLastAccessTime();

    unsigned char * getBuffer()
    {
        return buffer;
    }

    size_t getBufferSize()
    {
        return bufferSize;
    }

};

#endif // FILEMEMCACHEDATA_H
