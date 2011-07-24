#include "httpheader.h"

HttpHeader::HttpHeader()
{
}

HttpHeader::~HttpHeader()
{

}


HttpHeader::HttpHeader(const HttpHeader &in)
    :QObject(),
      httpMethod(in.httpMethod),
      body(in.body),
      headerInfo(in.headerInfo),
      fragment(in.fragment),
      queryString(in.queryString),
      path(in.path),
      host(in.host),
      url(in.url)
{

}


void HttpHeader::operator=(const HttpHeader &in)
{
    body=in.body;
    headerInfo=in.headerInfo;
    fragment=in.fragment;
    queryString=in.queryString;
    path=in.path;
    host=in.host;
    url=in.url;
}
