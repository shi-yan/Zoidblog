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
     int incommingCount;
     Q_OBJECT
 public:

    Worker *worker1;
    Worker *worker2;

     HttpServer(quint16 port, QObject* parent = 0);

     void incomingConnection(int socket);

     void pause()
     {
         disabled = true;
     }

     void resume()
     {
         disabled = false;
     }

 private:
     bool disabled;
 };

#endif // HTTPSERVER_H
