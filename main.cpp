#include <QtGui/QApplication>
#include "httpserver.h"
#include "zoidblog.h"
#include "taskhandler.h"
#include "pathtree.h"
#include "httprequest.h"
#include "httpresponse.h"

int main(int argc, char *argv[])
{



    REGISTER_WEBAPP(Zoidblog);

    QApplication a(argc, argv);

    HttpServer::getSingleton().start(3,8080);

    return a.exec();
}
