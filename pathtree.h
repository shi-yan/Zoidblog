#ifndef PATHTREE_H
#define PATHTREE_H

#include <QObject>
#include "pathtreenode.h"
#include "taskhandler.h"

class PathTree:public QObject
{
    Q_OBJECT

    PathTreeNode root;

    PathTree(QObject *parent=0);
public:

    static PathTree &getSingleton()
    {
        static PathTree obj;
        return obj;
    }

    bool registerAPath(const QString &path,QObject *object,const QString &methodName);
    const TaskHandler * getTaskHandlerByPath(const QString &path);
};

#endif // PATHTREE_H
