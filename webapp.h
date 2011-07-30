#ifndef WEBAPP_H
#define WEBAPP_H

#include <QObject>
#include "httpresponse.h"
#include "httprequest.h"
#include "pathtree.h"


class WebApp:public QObject
{
    Q_OBJECT

    const QString pathSpace;

    PathTree *pathTree;

public:
    WebApp(const QString &_pathSpace="",QObject *parent =0);
    WebApp(const WebApp &in):QObject(),pathSpace(in.pathSpace){}



    bool addGetHandler(const QString &_path,const QString &handlerName);
    bool addPostHandler(const QString &_path,const QString &handlerName);

    virtual ~WebApp(){}


    const QString & getPathSpace()
    {
        return pathSpace;
    }


    virtual void init(){}

    virtual void registerPathHandlers()=0;

    void setPathTree(PathTree *pt)
    {
        pathTree=pt;
    }
};

#endif // WEBAPP_H
