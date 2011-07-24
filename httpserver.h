#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include "worker.h"

class HttpServer : public QTcpServer
 {
     Q_OBJECT

    int connectionCount;

    bool disabled;

    HttpServer(QObject* parent = 0);

    void incomingConnection(int socket);

public:

    static HttpServer &getSingleton()
    {
        static HttpServer obj;
        return obj;
    }

    void start(int numOfWorkers,quint16 port);

     void pause()
     {
         disabled = true;
     }

     void resume()
     {
         disabled = false;
     }
 };

#endif // HTTPSERVER_H
