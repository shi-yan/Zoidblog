#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include "httpheader.h"

class TcpSocket:public QTcpSocket
{
    Q_OBJECT

    bool isNew;
    unsigned int totalBytes;
    unsigned int bytesHaveRead;

    QByteArray dataBuffer;
    HttpHeader header;

    QString rawHeader;

public:
    TcpSocket(QObject *parent=0);
    TcpSocket(const TcpSocket &in);
    void operator=(const TcpSocket &in);
    ~TcpSocket();

    void setRawHeader(const QString &in)
    {
        rawHeader=in;
    }

    QString & getRawHeader()
    {
        rawHeader;
    }

    unsigned int getTotalBytes()
    {
        return totalBytes;
    }

    unsigned int getBytesHaveRead()
    {
        return bytesHaveRead;
    }

    HttpHeader & getHeader()
    {
        return header;
    }

    void setHttpHeader(HttpHeader &_header)
    {
        header=_header;
    }

    bool isEof()
    {
        return (isNew==false) && (totalBytes<=bytesHaveRead);
    }

    void notNew()
    {
        isNew=false;
    }

    bool isNewSocket()
    {
        return isNew;
    }

    void setTotalBytes(unsigned int _totalBytes)
    {
        totalBytes=_totalBytes;
    }

    void appendData(const char* buffer,unsigned int size);
    void appendData(const QByteArray &buffer);

    QByteArray &getBuffer()
    {
        return dataBuffer;
    }


};

#endif // TCPSOCKET_H
