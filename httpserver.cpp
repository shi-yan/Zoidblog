#include "httpserver.h"
#include "httpheader.h"
#include "worker.h"
#include "incommingconnectionqueue.h"

HttpServer::HttpServer(QObject* parent )
    : QTcpServer(parent), disabled(false),connectionCount(0)
{
}

void HttpServer::incomingConnection(int socket)
{
    if (disabled)
        return;

    InCommingConnectionQueue::getSingleton().insertATask(socket);

    qDebug()<<"New Connection!"<<socket;

    connectionCount++;
}

void HttpServer::start(int numOfWorkers,quint16 port)
{
    qDebug()<<"Need to create"<<numOfWorkers<<"workers";

    if(numOfWorkers<1)
        numOfWorkers=1;

    for(int i=0;i<numOfWorkers;++i)
    {
        Worker *aWorker=new Worker(QString("worker %1").arg(i));

        aWorker->start();
        aWorker->moveToThread(aWorker);
    }

    listen(QHostAddress::Any, port);

    qDebug()<<"Start listening! main ThreadId"<<thread()->currentThreadId();
}
