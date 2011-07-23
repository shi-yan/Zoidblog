#ifndef PATHTREENODE_H
#define PATHTREENODE_H

#include <QObject>
#include "taskhandler.h"
#include <QMap>

class PathTreeNode:public QObject
{
    Q_OBJECT

    QString pathName;
    TaskHandler taskHandler;
    QMap<QString,PathTreeNode> children;

public:
    PathTreeNode();
    PathTreeNode(const PathTreeNode &in);
    PathTreeNode(const QString _pathName,QObject *object=0,const QString methodName="");
    void operator=(const PathTreeNode &in);


    void addChild(const QString &childPathName);
    bool hasChild(const QString &childPathName);

    PathTreeNode *getChild(const QString &childePathName)
    {
        return &children[childePathName];
    }

    bool setTaskHandler(QObject *object,const QString methodName);

    const TaskHandler * getTaskHandler()
    {
        return &taskHandler;
    }

    QString & getPathName()
    {
        return pathName;
    }


};

#endif // PATHTREENODE_H
