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
#include <QImage>
#include <QPainter>


int onMessageBegin(http_parser *)
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

int onMessageComplete(http_parser *)
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
        else if(nparsed!=(size_t)inCommingContent.count())
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


        QString contentLengthString=request.getHeader().getHeaderInfo("Content-Length");
        if(!contentLengthString.isEmpty())
        {
            //int contentLength=contentLengthString.toInt();

            QString contentTypeString=request.getHeader().getHeaderInfo("Content-Type");

            if(!contentTypeString.isEmpty())
            {
                int i=0;
                for(i=0;i<contentTypeString.count();++i)
                {
                    if(contentTypeString.at(i)==';')
                        break;
                }

                QString contentType=contentTypeString.mid(0,i);

                if(contentType=="multipart/form-data")
                {
                    qDebug()<<"found form data!";



                    if(contentTypeString.mid(i+2,9)=="boundary=")
                    {
                        QString boundary=contentTypeString.mid(i+11,contentTypeString.count()-i-11);

                        qDebug()<<"And the boundary is:"<<boundary;



                        QByteArray body=request.getHeader().getBody();

                        QMap<QString,QByteArray> formData;

                        int linebegin=0;
                        int lineend=0;

                        bool success=false;

                        while(body.at(lineend)!='\r' && body.at(lineend+1)!='\n')
                        {
                            ++lineend;
                        }

                        QString boundaryCheck= QByteArray(&body.data()[linebegin],lineend-linebegin);



                        if(boundaryCheck=="--"+boundary)
                        {
                            lineend+=2;
                            linebegin=lineend;
                            while(lineend<body.count())
                            {
                                while(lineend<body.count()-1 && body.at(lineend)!='\r' && body.at(lineend+1)!='\n')
                                {
                                    ++lineend;
                                }

                                QString fieldCheck= QByteArray(&body.data()[linebegin],lineend-linebegin);

                                if(!(fieldCheck.left(38)=="Content-Disposition: form-data; name=\""))
                                {
                                    break;
                                }

                                int namelength=38;

                                while(fieldCheck.at(namelength)!='\"' && namelength<fieldCheck.count())
                                {
                                    ++namelength;
                                }

                                QString fieldName=fieldCheck.mid(38,namelength-38);

                                //qDebug()<<"Field name:"<<fieldName;


                                lineend+=2;
                                linebegin=lineend;

                                if(lineend>=body.count())
                                    break;

                                while(lineend<body.count()-1 && body.at(lineend)!='\r' && body.at(lineend+1)!='\n')
                                {
                                    ++lineend;
                                }

                                lineend+=2;
                                linebegin=lineend;

                                QByteArray value;

                                while(lineend<body.count())
                                {
                                    while(lineend<body.count()-1 && body.at(lineend)!='\r' && body.at(lineend+1)!='\n')
                                    {
                                        ++lineend;
                                    }

                                    QByteArray thisline(&body.data()[linebegin],lineend-linebegin);

                                    QString aValueLine=thisline;

                                  //  qDebug()<<aValueLine;

                                    if(aValueLine.left(2+boundary.count())=="--"+boundary)
                                    {

                                     //   qDebug()<<"Value:"<<value;

                                        formData[fieldName]=value;

                                        if(aValueLine.right(2)=="--")
                                        {
                                                success=true;

                                        }


                                        lineend+=2;
                                        linebegin=lineend;



                                        break;
                                    }

                                    value.append(thisline);
                                    value.append('\r');
                                    value.append('\n');

                                    lineend+=2;
                                    linebegin=lineend;


                                }
                        }


                    }


                        if(success)
                        {
                            qDebug()<<"form data retrieved successfully!";
request.setFormData(formData);
                         //   qDebug()<<formData;
                        }

                    }
                }
            }
        }























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



        QDataStream os(socket);


    /*   os << "HTTP/1.0 200 Ok\r\n"
            "Content-Type: text/html; charset=\"utf-8\"\r\n"
            "\r\n";
      */ //     "<h1>Nothing to see here</h1>\n";
     /*       << QDateTime::currentDateTime().toString() << "\n"
            <<"received:<br/>"<<workerName<<"<br/>\r\n"<<
              request.getHeader().toString()<<"<br/>"
           <<response.debugInfo<<"<br/>"
          <<thread()->currentThreadId();


        qDebug()<<response.debugInfo;*/

       // os<<response.getBuffer();

        FILE *file;
        char *buffer;

        unsigned long fileLen;

        file=fopen("/home/shi/Desktop/Screenshot-6.png","rb");

        if(!file)
        {
            qDebug()<<"unable to open file.";
        }

        fseek(file,0,SEEK_END);
        fileLen=ftell(file);
        fseek(file,0,SEEK_SET);

        buffer=(char *)malloc(fileLen+1);

        if(!buffer)
        {
            qDebug()<<"Memory error!";
            fclose(file);
        }

        fread(buffer,fileLen,1,file);

        fclose(file);




    //    QByteArray qba(buffer,fileLen+1);


     //   os.writeRawData(buffer,fileLen+1);

       QImage aaa(400,200,QImage::Format_Mono);

       aaa.fill(0xF);
       QPainter painter(&aaa);

       painter.setFont(QFont("Bengali",30));
        painter.drawText(QRectF(10,10,300,180),"DAFafklfa;");

        aaa.save(socket,"png");


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
