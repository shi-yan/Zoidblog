#include "zoidblog.h"
#include <QThread>

#include <QDebug>
#include <QImage>
#include <QBuffer>

Zoidblog::Zoidblog():WebApp("")
{
}

Zoidblog::~Zoidblog()
{
}

void Zoidblog::registerHandlers()
{
    bool result=addGetHandler("/path","handlePathGet");

    result |= addGetHandler("/test.png","handleTestGet");

    result |= addPostHandler("/path","handlePathPost");

    qDebug()<<"result of register handler:"<<result;
}

void Zoidblog::handlePathGet(HttpRequest &,HttpResponse &response)
{
 //  response.debugInfo+=request.getHeader().toString();
   //response.debugInfo+=QString("this is inside zoidblog!!! by thread:%1").arg((int)(thread()->currentThreadId()));
   //thread()->currentThreadId();

    response.getBuffer().clear();

        response.getBuffer().append("<form action=\"/path\" method=\"post\" enctype=\"Multipart/form-data\" >");
        response.getBuffer().append("<input type=\"text\" name=\"field1\">");
        response.getBuffer().append("<input type=\"text\" name=\"field2\">");
        response.getBuffer().append("<textarea type=\"text\" name=\"field3\" rows=\"2\" cols=\"20\"></textarea>");
       response.getBuffer().append("<input type=\"submit\" value=\"Submit\">");
        response.getBuffer().append("</form>");
}

void Zoidblog::handleTestGet(HttpRequest &request,HttpResponse &response)
{
    qDebug()<<"inside test!!!!!!!!!!!!!"<<request.debugInfo<<response.debugInfo;

    QImage test("/home/shi/Desktop/Screenshot-6.png");
    QFile testpng("/home/shi/Desktop/Screenshot-6.png");
    testpng.open(QIODevice::ReadOnly);

    QDataStream in(&testpng);



    testpng.close();

    response.getBuffer().append(imagebuffer);
}

void Zoidblog::handlePathPost(HttpRequest &request,HttpResponse &response)
{
    qDebug()<<"inside post path test!!";

    if(request.hasFormData())
    {
        qDebug()<<"field1:"<<request.getFormData("field1");
        qDebug()<<"field2:"<<request.getFormData("field2");
        qDebug()<<"field3:"<<request.getFormData("field3");
response.getBuffer().clear();
             response.getBuffer().append("found Form Data:<br/>");
             response.getBuffer().append("field1:"+QString::fromUtf8(request.getFormData("field1").data(),request.getFormData("field1").count())+"<br/>");
         response.getBuffer().append("field2:"+request.getFormData("field2")+"<br/>");
             response.getBuffer().append("field3:"+request.getFormData("field3")+"<br/>");






    }
}

void Zoidblog::handleTestPost(HttpRequest &,HttpResponse &)
{

}
