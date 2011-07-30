#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#define REGISTER_WEBAPP( webapp ) \
    do { \
      HttpServer::getSingleton().registerWebApp<webapp>("webapp");\
    } while (0)

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

    QVector<int> webAppSet;

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

     template<class T>
     void registerWebApp(const char * webAppName)
     {
         int id = qRegisterMetaType<T>(webAppName);
        webAppSet.push_back(id);
     }

 };

#endif // HTTPSERVER_H
