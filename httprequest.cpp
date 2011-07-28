#include "httprequest.h"

HttpRequest::HttpRequest(TcpSocket *_socket):QObject(),header(),hasSetFormData(false),formData(),socket(_socket)
{
}

HttpRequest::HttpRequest(const HttpRequest &in):QObject(),header(in.header),debugInfo(in.debugInfo),hasSetFormData(in.hasSetFormData),formData(in.formData)
{
}

void HttpRequest::operator=(const HttpRequest &in)
{
    header=in.header;
    debugInfo=in.debugInfo;
    hasSetFormData=in.hasSetFormData;
    formData=in.formData;
}


HttpRequest::~HttpRequest()
{

}

void HttpRequest::appendData(const char*,unsigned int)
{
    rawData.append(buffer,size);
    bytesHaveRead+=size;
}

void HttpRequest::appendData(QByteArray &ba)
{
    rawData.append(ba);
    bytesHaveRead+=ba.count();
}

void HttpRequest::setRawHeader(const QString &_rh)
{
    rawHeader=_rh;
}
