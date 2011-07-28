#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent):QTcpSocket(parent),isNew(true),request(this),response(this)
{
}

TcpSocket::TcpSocket(const TcpSocket &in):QTcpSocket(),totalBytes(in.totalBytes),bytesHaveRead(in.bytesHaveRead),dataBuffer(in.dataBuffer),header(in.header),rawHeader(in.rawHeader),isNew(in.isNew)
{
    setSocketDescriptor(in.socketDescriptor());
}

void TcpSocket::operator=(const TcpSocket &in)
{
    (*this)=in;
    totalBytes=in.totalBytes;
    bytesHaveRead=in.bytesHaveRead;
    dataBuffer=in.dataBuffer;
    header=in.header;
    rawHeader=in.rawHeader;
    isNew=in.isNew;
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::appendData(const char* buffer,unsigned int size)
{
    request.appendData(buffer,size);
}

void TcpSocket::appendData(const QByteArray &buffer)
{
    request.appendData(buffer);
}

void TcpSocket::setRawHeader(const QString &in)
{
    request.setRawHeader(in);
}

QString & TcpSocket::getRawHeader()
{
    rawHeader;
}

unsigned int TcpSocket::getTotalBytes()
{
    return totalBytes;
}

unsigned int TcpSocket::getBytesHaveRead()
{
    return bytesHaveRead;
}

HttpHeader & TcpSocket::getHeader()
{
    return header;
}

void TcpSocket::setHttpHeader(HttpHeader &_header)
{
    header=_header;
}

bool TcpSocket::isEof()
{
    return (isNew==false) && (totalBytes<=bytesHaveRead);
}

void TcpSocket::notNew()
{
    isNew=false;
}

bool TcpSocket::isNewSocket()
{
    return isNew;
}

void TcpSocket::setTotalBytes(unsigned int _totalBytes)
{
    totalBytes=_totalBytes;
}

QByteArray &TcpSocket::getBuffer()
{
    return dataBuffer;
}
