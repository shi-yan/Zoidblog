#ifndef ZOIDBLOG_H
#define ZOIDBLOG_H

#include "webapp.h"

class Zoidblog:public WebApp
{
    Q_OBJECT

public:
    Zoidblog();

    ~Zoidblog();

    void registerHandlers();

public slots:
    void handlePath(HttpRequest &,HttpResponse &);
    void handleTest(HttpRequest &,HttpResponse &);
};

#endif // ZOIDBLOG_H
