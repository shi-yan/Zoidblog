#ifndef ZOIDBLOG_H
#define ZOIDBLOG_H

#include "webapp.h"

class Zoidblog:public WebApp
{
    Q_OBJECT



public:
    Zoidblog();
    Zoidblog(const Zoidblog &):WebApp(){}
    void registerPathHandlers();
    void init();

public slots:
    void handlePathGet(HttpRequest &,HttpResponse &);
    void handleTestGet(HttpRequest &,HttpResponse &);

    void handlePathPost(HttpRequest &,HttpResponse &);
    void handleTestPost(HttpRequest &,HttpResponse &);

    void handleImageUploadGet(HttpRequest &,HttpResponse &);
    void handleImageUploadPost(HttpRequest &,HttpResponse &);
};

#endif // ZOIDBLOG_H
