#include "zoidblog.h"
#include <QThread>

#include <QDebug>

Zoidblog::Zoidblog():WebApp("")
{
}

Zoidblog::~Zoidblog()
{
}

void Zoidblog::registerHandlers()
{
    bool result=addGetHandler("/path","handlePathGet");

    result |= addGetHandler("/test","handleTestGet");

    result |= addPostHandler("/path","handlePathPost");

    qDebug()<<"result of register handler:"<<result;
}

void Zoidblog::handlePathGet(HttpRequest &request,HttpResponse &response)
{
 //  response.debugInfo+=request.getHeader().toString();
   //response.debugInfo+=QString("this is inside zoidblog!!! by thread:%1").arg((int)(thread()->currentThreadId()));
   //thread()->currentThreadId();


   response.debugInfo+="<form action=\"/path\" method=\"post\" enctype=\"multipart/form-data\" >";
   response.debugInfo+="<input type=\"text\" name=\"field1\">";
   response.debugInfo+="<input type=\"text\" name=\"field2\">";
   response.debugInfo+="<input type=\"submit\" value=\"Submit\">";
   response.debugInfo+="</form>";
}

void Zoidblog::handleTestGet(HttpRequest &request,HttpResponse &response)
{
    qDebug()<<"inside test!!!!!!!!!!!!!"<<request.debugInfo<<response.debugInfo;
}

void Zoidblog::handlePathPost(HttpRequest &,HttpResponse &)
{
    qDebug()<<"inside post path test!!";
}

void Zoidblog::handleTestPost(HttpRequest &,HttpResponse &)
{

}
