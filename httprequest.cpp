#include "httprequest.h"

HttpRequest::HttpRequest(TcpSocket *_socket)
    :QObject(),
      header(),
      rawData(),
      formData(),
      hasSetFormData(false),
      totalBytes(0),
      bytesHaveRead(0),
      rawHeader(),
      socket(_socket)
{
}

HttpRequest::HttpRequest(const HttpRequest &in)
    :QObject(),
      header(in.header),
      rawData(in.rawData),
      formData(in.formData),
      hasSetFormData(in.hasSetFormData),
      totalBytes(in.totalBytes),
      bytesHaveRead(in.bytesHaveRead),
      rawHeader(in.rawHeader),
      socket(in.socket)
{
}

void HttpRequest::operator=(const HttpRequest &in)
{
    header=in.header;
    rawData=in.rawData;
    formData=in.formData;
    hasSetFormData=in.hasSetFormData;
    totalBytes=in.totalBytes;
    bytesHaveRead=in.bytesHaveRead;
    rawHeader=in.rawHeader;
    socket=in.socket;
}


HttpRequest::~HttpRequest()
{

}

void HttpRequest::appendData(const char* buffer,unsigned int size)
{
    rawData.append(buffer,size);
    bytesHaveRead+=size;
}

void HttpRequest::appendData(const QByteArray &ba)
{
    rawData.append(ba);
    bytesHaveRead+=ba.count();
}

void HttpRequest::setRawHeader(const QString &_rh)
{
    rawHeader=_rh;
}
