#include "worker.h"
#include <QDebug>
#include <QTcpSocket>
#include <QDateTime>
#include "httpheader.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "pathtree.h"
#include "incommingconnectionqueue.h"
#include <QtCore/QCoreApplication>


int onMessageBegin(http_parser *parser)
{
    qDebug()<<"Parse Message Begin";
    return 0;
}

int onPath(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
    //qDebug()<<"onPath:"<<QString(buffer);

    ((HttpHeader*)parser->data)->setPath(QString(buffer));

    return 0;
}

int onQueryString(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
   // qDebug()<<"onQueryString:"<<QString(buffer);
    ((HttpHeader*)parser->data)->setQueryString(QString(buffer));
    return 0;
}

int onUrl(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
    //qDebug()<<"onUrl:"<<QString(buffer);
    ((HttpHeader*)parser->data)->setUrl(QString(buffer));
    return 0;
}

int onFragment(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
  //  qDebug()<<"onFragment:"<<QString(buffer);

    ((HttpHeader*)parser->data)->setFragment(QString(buffer));
    return 0;
}

int onHeaderField(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
  //  qDebug()<<"onHeaderField:"<<QString(buffer);
    ((HttpHeader*)parser->data)->setCurrentHeaderField(QString(buffer));
    return 0;
}

int onHeaderValue(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
   // qDebug()<<"onHeaderValue:"<<QString(buffer);
    ((HttpHeader*)parser->data)->addHeaderInfo(QString(buffer));
    return 0;
}

int onHeadersComplete(http_parser *parser)
{
    ((HttpHeader*)parser->data)->setHost(((HttpHeader*)parser->data)->getHeaderInfo("Host"));
    //qDebug()<<"Parse Header Complete";
    return 0;
}

int onBody(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
    //qDebug()<<"onBody:"<<QString(buffer);
    ((HttpHeader*)parser->data)->setBody(buffer);
    return 0;
}

int onMessageComplete(http_parser *parser)
{
    qDebug()<<"Parse Message Complete";
    return 0;
}



Worker::Worker(const QString _name)
{
   workerName=_name;
}

void Worker::newSocket(int socket)
{
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this    , SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}


void Worker::readClient()
{
    //if (disabled)
    //    return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {
        HttpRequest request;
        HttpResponse response;

        QByteArray inCommingContent=socket->readAll();

        http_parser_settings settings;

        settings.on_message_begin=onMessageBegin;
        settings. on_path=onPath;
        settings. on_query_string=onQueryString;
        settings. on_url=onUrl;
        settings. on_fragment=onFragment;
        settings. on_header_field=onHeaderField;
        settings. on_header_value=onHeaderValue;
        settings. on_headers_complete=onHeadersComplete;
        settings. on_body=onBody;
        settings. on_message_complete=onMessageComplete;

        http_parser_init(&parser,HTTP_REQUEST);
        parser.data = &request.getHeader();

        qDebug()<<"before execution. Buffer size:"<<inCommingContent.count();

        size_t nparsed = http_parser_execute(&parser,&settings,inCommingContent.constData(),inCommingContent.count());

        qDebug()<<"finish execution";

        if(parser.upgrade)
        {
            qDebug()<<"upgrade";
        }
        else if(nparsed!=inCommingContent.count())
        {
            qDebug()<<"nparsed:"<<nparsed<<"buffer size:"<<inCommingContent.count();
        }
        else
        {
            qDebug()<<"parsing seems to be succeed!";

            qDebug()<<parser.method;
        }

        PathTreeNode::TaskHandlerType handlerType;

        if(parser.method==HTTP_GET)
            handlerType=PathTreeNode::GET;
        else if(parser.method==HTTP_POST)
            handlerType=PathTreeNode::POST;
        else
            socket->close();

        const TaskHandler *th=PathTree::getSingleton().getTaskHandlerByPath(request.getHeader().getPath(),handlerType);

        if(th )
        {
            if(!th->isEmpty())
            {
                if(th->invoke(request,response))
                    qDebug()<<"invoke successful!";
                else
                    qDebug()<<"invoke unsuccessful!";
            }
            else
                qDebug()<<"no task handler!";
        }
        else
            qDebug()<<"empty task handler!";



        QTextStream os(socket);

        os << "HTTP/1.0 200 Ok\r\n"
            "Content-Type: text/html; charset=\"utf-8\"\r\n"
            "\r\n"
            "<h1>Nothing to see here</h1>\n"
            << QDateTime::currentDateTime().toString() << "\n"
            <<"received:<br/>"<<workerName<<"<br/>\r\n"<<
              request.getHeader().toString()<<"<br/>"
           <<response.debugInfo<<"<br/>"
          <<thread()->currentThreadId();


        qDebug()<<response.debugInfo;

        qDebug()<<"before closeing";
        socket->close();


    }

}

void Worker::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
    qDebug()<<"discard client inside worker";
}

void Worker::run()
{
    qDebug()<<workerName<<"'s thread id"<<thread()->currentThreadId();

    forever {


        int socket=InCommingConnectionQueue::getSingleton().getATask();

        if(socket!=-1)
        {
            newSocket(socket);
        }

         QCoreApplication::processEvents();


    }

    exec();
}
