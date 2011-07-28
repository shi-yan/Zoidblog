#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent):QTcpSocket(parent),totalBytes(0),bytesHaveRead(0),dataBuffer(),header(),rawHeader(),isNew(true)
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
    dataBuffer.append(buffer,size);
    bytesHaveRead+=size;
}

void TcpSocket::appendData(const QByteArray &buffer)
{
    dataBuffer.append(buffer);
    bytesHaveRead+=buffer.count();
}
