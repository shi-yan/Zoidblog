#include "pathtreenode.h"

PathTreeNode::PathTreeNode()
    :QObject(),
      pathName(),
      taskHandler(),
      children()
{
}

PathTreeNode::PathTreeNode(const PathTreeNode &in)
    :QObject(),
      pathName(in.pathName),
      taskHandler(in.taskHandler),
      children()
{
    children=in.children;
}

PathTreeNode::PathTreeNode(const QString _pathName,QObject *object,const QString methodName)
    :QObject(),
      pathName(_pathName),
      taskHandler(),
      children()
{
    if(object)
    {
        taskHandler.setHandler(object,methodName);
    }
}

void PathTreeNode::operator=(const PathTreeNode &in)
{
    pathName=in.pathName;
    taskHandler=in.taskHandler;
    children=in.children;
}


void PathTreeNode::addChild(const QString &childPathName)
{
    PathTreeNode newNode(childPathName);
    children[childPathName]=newNode;
}

bool PathTreeNode::hasChild(const QString &childPathName)
{
    return children.contains(childPathName);
}

bool PathTreeNode::setTaskHandler(QObject *object,const QString methodName)
{
    if(taskHandler.isEmpty())
    {
        return taskHandler.setHandler(object,methodName);
    }
    else
        return false;
}
