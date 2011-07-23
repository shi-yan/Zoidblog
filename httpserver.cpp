#include "httpserver.h"
#include "httpheader.h"
#include "worker.h"

HttpServer::HttpServer(quint16 port, QObject* parent )
    : QTcpServer(parent), disabled(false),incommingCount(0)
{
    qDebug()<<"before listening!";
    worker1=new Worker("Worker 1");
    worker1->start();
    worker2=new Worker("Worker 2");
    worker2->start();
    listen(QHostAddress::Any, port);
    qDebug()<<"listening!";

    srand(12341234);
}

void HttpServer::incomingConnection(int socket)
{
    if (disabled)
        return;

    incommingCount=rand()%3;

    qDebug()<<"incomming count"<<incommingCount;


    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), incommingCount%2?worker1:worker2    , SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), incommingCount%2?worker1:worker2 , SLOT(discardClient()));
    s->setSocketDescriptor(socket);

    //QtServiceBase::instance()->logMessage("New Connection");
    qDebug()<<"New Connection!"<<socket;

    incommingCount++;
}
