#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QDebug>

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
     void readClient()
     {
         if (disabled)
             return;

         // This slot is called when the client sent data to the server. The
         // server looks if it was a get request and sends a very simple HTML
         // document back.
         QTcpSocket* socket = (QTcpSocket*)sender();
         if (socket->canReadLine()) {

             QString firstLine;

             QStringList tokens = (firstLine=QString(socket->readLine())).split(QRegExp("[ \r\n][ \r\n]*"));

             qDebug()<<firstLine;

             if (tokens[0] == "GET") {




                 QTextStream os(socket);
                 os.setAutoDetectUnicode(true);
                 os << "HTTP/1.0 200 Ok\r\n"
                     "Content-Type: text/html; charset=\"utf-8\"\r\n"
                     "\r\n"
                     "<h1>Nothing to see here</h1>\n"
                     << QDateTime::currentDateTime().toString() << "\n"
                     <<"received:<br/>"
                    <<firstLine<<"<br/>";

                 while(socket->canReadLine())
                 {
                     os<<socket->readLine()<<"<br>";
                 }


                 socket->close();

               //  QtServiceBase::instance()->logMessage("Wrote to client");

                 if (socket->state() == QTcpSocket::UnconnectedState) {
                     delete socket;
                    // QtServiceBase::instance()->logMessage("Connection closed");
                 }
             }
         }
     }
     void discardClient()
     {
         QTcpSocket* socket = (QTcpSocket*)sender();
         socket->deleteLater();

        // QtServiceBase::instance()->logMessage("Connection closed");
     }

 private:
     bool disabled;
 };

#endif // HTTPSERVER_H
