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

    Zoidblog blog;
    blog.registerHandlers();

    QCoreApplication a(argc, argv);

    HttpServer::getSingleton().start(3,8080);

    return a.exec();
}
