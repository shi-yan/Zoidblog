#include "zoidblog.h"

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
    qDebug()<<"inside path!!!!!!!!!!!!!"<<request.debugInfo<<response.debugInfo;
}

void Zoidblog::handleTest(HttpRequest &request,HttpResponse &response)
{
    qDebug()<<"inside test!!!!!!!!!!!!!"<<request.debugInfo<<response.debugInfo;
}
