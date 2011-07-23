#include "httpresponse.h"

HttpResponse::HttpResponse():QObject()
{
}

HttpResponse::HttpResponse(const HttpResponse &in)
    :QObject(),debugInfo(in.debugInfo)
{

}

void HttpResponse::operator=(const HttpResponse &in)
{
    debugInfo=in.debugInfo;
}

HttpResponse::~HttpResponse()
{

}

  void HttpResponse::operator<<(const QString &info)
  {
    buffer.append(info);
  }
