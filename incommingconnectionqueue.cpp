#include "incommingconnectionqueue.h"

InCommingConnectionQueue::InCommingConnectionQueue():QObject(),
    socketQueue(),newTaskComming(false),accessMutex(),hasNewSocket()
{   
}

InCommingConnectionQueue::~InCommingConnectionQueue()
{
}

