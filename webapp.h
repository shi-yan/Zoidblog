#ifndef WEBAPP_H
#define WEBAPP_H

#include <QObject>
#include "httpresponse.h"
#include "httprequest.h"


class WebApp:public QObject
{
    Q_OBJECT
    const QString pathSpace;

public:
    WebApp(const QString &_pathSpace="",QObject *parent =0);



    bool addGetHandler(const QString &_path,const QString &handlerName);
    bool addPostHandler(const QString &_path,const QString &handlerName);

    virtual ~WebApp(){}


    const QString & getPathSpace()
    {
        return pathSpace;
    }


        virtual void init()=0;
};

#endif // WEBAPP_H
