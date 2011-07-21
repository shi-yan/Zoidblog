#include <QtCore/QCoreApplication>
#include "httpserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


   HttpServer *daemon = new HttpServer(8080, &a);



    return a.exec();
}
