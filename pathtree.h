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

    bool registerAPath(const QString &path,QObject *object,const QString &methodName,enum PathTreeNode::TaskHandlerType);
    const TaskHandler * getTaskHandlerByPath(const QString &path,enum PathTreeNode::TaskHandlerType);
};

#endif // PATHTREE_H
