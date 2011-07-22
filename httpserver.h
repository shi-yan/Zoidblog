#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include "http_parser.h"



class HttpServer : public QTcpServer
 {
     Q_OBJECT
 public:
     HttpServer(quint16 port, QObject* parent = 0)
         : QTcpServer(parent), disabled(false)
     {
         listen(QHostAddress::Any, port);
     }

     void incomingConnection(int socket)
     {
         if (disabled)
             return;

         // When a new client connects, the server constructs a QTcpSocket and all
         // communication with the client is done over this QTcpSocket. QTcpSocket
         // works asynchronously, this means that all the communication is done
         // in the two slots readClient() and discardClient().
         QTcpSocket* s = new QTcpSocket(this);
         connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
         connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
         s->setSocketDescriptor(socket);

         //QtServiceBase::instance()->logMessage("New Connection");
         qDebug()<<"New Connection!";
     }

     void pause()
     {
         disabled = true;
     }

     void resume()
     {
         disabled = false;
     }

 private slots:
     void readClient();


     void discardClient()
     {
         QTcpSocket* socket = (QTcpSocket*)sender();
         socket->deleteLater();

        // QtServiceBase::instance()->logMessage("Connection closed");

         qDebug()<<"closed by discard function";
     }

 private:
     bool disabled;
 };

#endif // HTTPSERVER_H
