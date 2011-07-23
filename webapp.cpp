#include "webapp.h"
#include "pathtree.h"

WebApp::WebApp(const QString &_pathSpace,QObject *parent )
    :QObject(parent),
      pathSpace(_pathSpace)
{

}

bool WebApp::addGet(const QString &_path,const QString &handlerName)
{
    QString path=_path;

    if(!pathSpace.isNull() && !pathSpace.isEmpty())
        path='/' + pathSpace + _path;

    QString functionName=handlerName;


    return PathTree::getSingleton().registerAPath(path,this,functionName.append("(HttpRequest&,HttpResponse&)"));
}

