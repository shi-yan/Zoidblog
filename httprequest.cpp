#include "httprequest.h"

HttpRequest::HttpRequest():QObject(),header()
{
}

HttpRequest::HttpRequest(const HttpRequest &in):QObject(),header(in.header),debugInfo(in.debugInfo)
{
}

void HttpRequest::operator=(const HttpRequest &in)
{
    header=in.header;
    debugInfo=in.debugInfo;
}


HttpRequest::~HttpRequest()
{

}
