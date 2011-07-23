#include "httpserver.h"
#include "httpheader.h"
#include "worker.h"
#include "incommingconnectionqueue.h"

HttpServer::HttpServer(int numOfWorkers,quint16 port, QObject* parent )
    : QTcpServer(parent), disabled(false),incommingCount(0)
{
    qDebug()<<"before listening!";

    qDebug()<<"need to create"<<numOfWorkers<<"workers";

    if(numOfWorkers<1)
        numOfWorkers=1;

    for(int i=0;i<numOfWorkers;++i)
    {
        Worker *aWorker=new Worker(QString("worker %1").arg(i));

        aWorker->start();
        aWorker->moveToThread(aWorker);
    }

    listen(QHostAddress::Any, port);

    qDebug()<<"listening! threadid"<<thread()->currentThreadId();

    srand(12341234);
}

void HttpServer::incomingConnection(int socket)
{
    if (disabled)
        return;

    InCommingConnectionQueue::getSingleton().insertATask(socket);

    qDebug()<<"New Connection!"<<socket;

    incommingCount++;
}
