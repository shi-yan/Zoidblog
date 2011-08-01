#ifndef ZOIDBLOG_H
#define ZOIDBLOG_H

#include "webapp.h"
#include "pagetemplate.h"

class Zoidblog:public WebApp
{
    Q_OBJECT

    PageTemplate adminDashboardTemplate;


public:
    void registerPathHandlers();
    void init();

public slots:
    void handlePathGet(HttpRequest &,HttpResponse &);
    void handleTestGet(HttpRequest &,HttpResponse &);

    void handlePathPost(HttpRequest &,HttpResponse &);
    void handleTestPost(HttpRequest &,HttpResponse &);

    void handleImageUploadGet(HttpRequest &,HttpResponse &);
    void handleImageUploadPost(HttpRequest &,HttpResponse &);

    void adminGetHandler(HttpRequest &,HttpResponse &);
    void savePostHandler(HttpRequest&,HttpResponse&);
};

#endif // ZOIDBLOG_H
