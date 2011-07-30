#include "httpresponse.h"
#include "tcpsocket.h"


HttpResponse::HttpResponse(TcpSocket *_socket)
    :QObject()
  ,buffer()
  ,socket(_socket)
  ,header()
  ,statusCode(200)
{
}

HttpResponse::HttpResponse(const HttpResponse &in)
    :QObject()
    ,buffer(in.buffer)
    ,socket(in.socket)
    ,header(in.header)
    ,statusCode(in.statusCode)
{

}

void HttpResponse::operator=(const HttpResponse &in)
{
    buffer=in.buffer;
    socket=in.socket;
    header=in.header;
    statusCode=in.statusCode;
}

HttpResponse::~HttpResponse()
{

}



  HttpResponse& HttpResponse::operator<<(const QByteArray &in)
  {
    buffer.append(in);
    return *this;
  }

      HttpResponse& HttpResponse::operator<<(const char *in)
      {
        buffer.append(in);
        return *this;
      }


      HttpResponse& HttpResponse::operator<<(const QString &in)
      {
        buffer.append(in);
        return *this;
      }

  void HttpResponse::write(const char* in,const size_t size)
  {
    buffer.append(in,size);
  }

  void HttpResponse::finish(enum OutputType outputType)
  {
      switch(outputType)
      {
      case BINARY:
      {
          QDataStream bos(socket);
          bos<<buffer;
          break;
      }

      case TEXT:
      default:
      {
          QDataStream tos( socket );

          buffer.insert(0,header.toString());
          buffer.insert(0,QString("HTTP/1.1 %1 Ok\r\n"
                                             "Content-Type: text/html; charset=\"utf-8\"\r\n"
                                             "\r\n").arg(200));
          tos<<buffer;
          break;
      }

      }
  }
