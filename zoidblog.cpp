#include "zoidblog.h"
#include <QThread>

#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <cstdio>

 Zoidblog::Zoidblog():WebApp()
{

 }

void Zoidblog::init()
{
    bool result=addGetHandler("/path","handlePathGet");

    result |= addGetHandler("/test","handleTestGet");
result |=   addPostHandler("/test","handleTestPost");
    result |= addPostHandler("/path","handlePathPost");

    result |=addGetHandler("/imageupload","handleImageUploadGet");

    result |= addPostHandler("/imageupload","handleImageUploadPost");

    qDebug()<<"result of register handler:"<<result;
}

void Zoidblog::handlePathGet(HttpRequest &,HttpResponse &response)
{
    //response.debugInfo+=request.getHeader().toString();
    //response.debugInfo+=QString("this is inside zoidblog!!! by thread:%1").arg((int)(thread()->currentThreadId()));
    //thread()->currentThreadId();

  /*  response.getBuffer().clear();

        response.getBuffer().append("<form action=\"/path\" method=\"post\" enctype=\"Multipart/form-data\" >");
        response.getBuffer().append("<input type=\"text\" name=\"field1\">");
        response.getBuffer().append("<input type=\"text\" name=\"field2\">");
        response.getBuffer().append("<textarea type=\"text\" name=\"field3\" rows=\"2\" cols=\"20\"></textarea>");
       response.getBuffer().append("<input type=\"submit\" value=\"Submit\">");
        response.getBuffer().append("</form>");*/
}

void Zoidblog::handleTestGet(HttpRequest &,HttpResponse &response)
{
    response<<"<h2>nothing to see here!</h2><br/>\r\n";
    response<<"<form action=\"/test\" method=\"post\" enctype=\"Multipart/form-data\" >";
    response<<"<input type=\"text\" name=\"field1\">";
    response<<"<input type=\"text\" name=\"field2\">";
    response<<"<textarea type=\"text\" name=\"field3\" rows=\"2\" cols=\"20\"></textarea>";
    response<<"<input type=\"submit\" value=\"Submit\">";
    response<<"</form>";
    response.finish();
}

void Zoidblog::handlePathPost(HttpRequest &request,HttpResponse &response)
{

}

void Zoidblog::handleTestPost(HttpRequest &request,HttpResponse &response)
{
    response<<"inside post path test!!";

    if(request.hasFormData())
    {
        response<<"found Form Data:<br/>";
        response<<"field1:"<<QString::fromUtf8(request.getFormData("field1").data(),request.getFormData("field1").count()).append("<br/>");
        response<<"field2:"<<(request.getFormData("field2")+"<br/>");
        response<<"field3:"<<(request.getFormData("field3")+"<br/>");
    }

    response.finish();
}

void Zoidblog::handleImageUploadGet(HttpRequest &,HttpResponse &)
{
    qDebug()<<"inside image upload get";
}

void Zoidblog::handleImageUploadPost(HttpRequest &request,HttpResponse &)
{

}
