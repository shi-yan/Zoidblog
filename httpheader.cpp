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
      //body(in.body),
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
  //  body=in.body;
    headerInfo=in.headerInfo;
    fragment=in.fragment;
    queryString=in.queryString;
    path=in.path;
    host=in.host;
    url=in.url;
}

QTextStream & operator<<(QTextStream &ts,  HttpHeader &in)
{
    QMapIterator<QString, QString> i(in.getHeaderInfo());
    while (i.hasNext())
    {
        i.next();
        ts << i.key() << ": " << i.value() << "\r\n";
    }
    return ts;
}
