#include <QtCore/QCoreApplication>
#include "httpserver.h"
#include "zoidblog.h"
#include "taskhandler.h"
#include "pathtree.h"
#include "httprequest.h"
#include "httpresponse.h"

int main(int argc, char *argv[])
{

    qRegisterMetaType<HttpRequest>("HttpRequest");
    qRegisterMetaType<HttpResponse>("HttpResponse");




    QCoreApplication a(argc, argv);


 //  HttpServer *daemon = new HttpServer(8080, &a);

    Zoidblog *blog=new Zoidblog();


    blog->registerHandlers();

    const TaskHandler *th=PathTree::getSingleton().getTaskHandlerByPath("/path");

    HttpRequest request;
    HttpResponse response;

    request.debugInfo="this is a request!!!";
    response.debugInfo="this is a response";

    if(th )
    {
        if(!th->isEmpty())
        {
            if(th->invoke(request,response))
                qDebug()<<"invoke successful!";
            else
                qDebug()<<"invoke unsuccessful!";
        }
        else
            qDebug()<<"no task handler!";
    }
    else
        qDebug()<<"empty task handler!";


    return a.exec();
}
