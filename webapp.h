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
    WebApp(const QString &_pathSpace,QObject *parent =0);

    virtual void registerHandlers() = 0;

    bool addGet(const QString &_path,const QString &handlerName);

    virtual ~WebApp(){}

    const QString & getPathSpace()
    {
        return pathSpace;
    }
};

#endif // WEBAPP_H
