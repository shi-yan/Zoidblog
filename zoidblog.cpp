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
    bool result=addGet("/path","handlePath");

    result |= addGet("/test","handleTest");

    qDebug()<<"result of register handler:"<<result;
}

void Zoidblog::handlePath(HttpRequest &request,HttpResponse &response)
{
   response.debugInfo+=request.getHeader().toString();
   response.debugInfo+=QString("this is inside zoidblog!!! by thread:%1").arg(thread()->currentThreadId());
   //thread()->currentThreadId();


}

void Zoidblog::handleTest(HttpRequest &request,HttpResponse &response)
{
    qDebug()<<"inside test!!!!!!!!!!!!!"<<request.debugInfo<<response.debugInfo;
}
