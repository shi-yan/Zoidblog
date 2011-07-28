#include "worker.h"
#include <QDebug>
#include "tcpsocket.h"
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
   // qDebug()<<"Parse Message Begin";
    return 0;
}

int onPath(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
  //  qDebug()<<"onPath:"<<QString(buffer);

    ((TcpSocket*)parser->data)->getHeader().setPath(QString(buffer));

    return 0;
}

int onQueryString(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
   // qDebug()<<"onQueryString:"<<QString(buffer);
    ((TcpSocket*)parser->data)->getHeader().setQueryString(QString(buffer));
    return 0;
}

int onUrl(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
   // qDebug()<<"onUrl:"<<QString(buffer);
    ((TcpSocket*)parser->data)->getHeader().setUrl(QString(buffer));
    return 0;
}

int onFragment(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
  //  qDebug()<<"onFragment:"<<QString(buffer);

    ((TcpSocket*)parser->data)->getHeader().setFragment(QString(buffer));
    return 0;
}

int onHeaderField(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
  //  qDebug()<<"onHeaderField:"<<QString(buffer);
    ((TcpSocket*)parser->data)->getHeader().setCurrentHeaderField(QString(buffer));
    return 0;
}

int onHeaderValue(http_parser *parser, const char *p,size_t len)
{
    QByteArray buffer(p,len);
  //  qDebug()<<"onHeaderValue:"<<QString(buffer);
    ((TcpSocket*)parser->data)->getHeader().addHeaderInfo(QString(buffer));
    return 0;
}

int onHeadersComplete(http_parser *parser)
{
    ((TcpSocket*)parser->data)->getHeader().setHost(((TcpSocket*)parser->data)->getHeader().getHeaderInfo("Host"));
  // qDebug()<<"Parse Header Complete";
    return 0;
}

int onBody(http_parser *parser, const char *p,size_t len)
{
   // QByteArray buffer(p,len);
  //  qDebug()<<"onBody:"<<QString(buffer);
    ((TcpSocket*)parser->data)->appendData(p,len);
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
    TcpSocket* s = new TcpSocket(this);
    connect(s, SIGNAL(readyRead()), this    , SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}


bool Worker::parseFormData(const QString &contentTypeString,const QByteArray &_body,QMap<QString,QByteArray> &formData)
{
    bool success=false;

    int i=0;
    for(i=0;i<contentTypeString.count();++i)
    {
        if(contentTypeString.at(i)==';')
            break;
    }

    qDebug()<<"found form data!";

    if(contentTypeString.mid(i+2,9)=="boundary=")
    {
        QString boundary=contentTypeString.mid(i+11,contentTypeString.count()-i-11);

        qDebug()<<"And the boundary is:"<<boundary;

        QByteArray body=_body;

        QMap<QString,QByteArray> formData;

        int linebegin=0;
        int lineend=0;

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

                    if(aValueLine.left(2+boundary.count())=="--"+boundary)
                    {

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
    }

    return success;
}

void Worker::readClient()
{
    //if (disabled)
    //    return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    TcpSocket* socket = (TcpSocket*)sender();

    if (socket->bytesAvailable() && socket->isNewSocket())
    {
        QByteArray inCommingContent=socket->readAll();

        http_parser_settings settings;

        settings. on_message_begin=onMessageBegin;
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

        parser.data = socket;

        size_t nparsed = http_parser_execute(&parser,&settings,inCommingContent.constData(),inCommingContent.count());

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
        }

        socket->setRawHeader(inCommingContent);

        bool isBodySizeOK=false;
        unsigned int bodySize=socket->getHeader().getHeaderInfo("Content-Length").toUInt(&isBodySizeOK);

        if(isBodySizeOK==false)
        {
            bodySize=0;
        }

        socket->setTotalBytes(bodySize);

        socket->notNew();

        if(socket->isEof())
        {
            PathTreeNode::TaskHandlerType handlerType;

            if(parser.method==HTTP_GET)
            {
                socket->getHeader().setHttpMethod(HttpHeader::HTTP_GET);
                handlerType=PathTreeNode::GET;
            }
            else if(parser.method==HTTP_POST)
            {
                socket->getHeader().setHttpMethod(HttpHeader::HTTP_POST);
                handlerType=PathTreeNode::POST;
            }
            else
            {
                qDebug()<<"not get and post"<<socket->atEnd()<<socket->bytesAvailable()<<socket->ConnectedState;
                qDebug()<<inCommingContent;
                socket->close();
                return;
            }

            if(socket->getHeader().getPath().left(6)=="/file/")
            {
                qDebug()<<"serving binary!";
                FILE *file;
                char *buffer;

                unsigned long fileLen;

                file=fopen((QString("file/")+socket->getHeader().getPath().right(socket->getHeader().getPath().count()-6)).toStdString().c_str(),"rb");

                if(!file)
                {
                    qDebug()<<"unable to open file.";
                    socket->close();

                    return;
                }

                fseek(file,0,SEEK_END);
                fileLen=ftell(file);
                fseek(file,0,SEEK_SET);

                buffer=(char *)malloc(fileLen+1);

                if(!buffer)
                {
                    qDebug()<<"Memory error!";
                    fclose(file);
                    socket->close();
                    return;
                }

                fread(buffer,fileLen,1,file);

                fclose(file);

                QDataStream os(socket);
                os.writeRawData(buffer,fileLen);

                free(buffer);

                socket->close();
            }
            else
            {
                HttpRequest request;
                HttpResponse response;

                request.getHeader()=socket->getHeader();


                QString contentLengthString=socket->getHeader().getHeaderInfo("Content-Length");

                if(!contentLengthString.isEmpty())
                {
                    //int contentLength=contentLengthString.toInt();

                    QString contentTypeString=socket->getHeader().getHeaderInfo("Content-Type");

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
                            QMap<QString,QByteArray> formData;
                            bool success=parseFormData(contentType,socket->getBuffer(),formData);

                            if(success)
                            {
                                request.setFormData(formData);
                            }
                            else
                            {
                                socket->close();
                                return;
                            }
                        }
                        else if(contentTypeString=="application/octet-stream")
                        {
                            // while(1){};
                            qDebug()<<"uploading things!!";
                            //return;
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

                QTextStream os(socket);
                os<<"{success:true}";
                qDebug()<<"before closeing";

                socket->close();
            }
        }
        else{
            qDebug()<<"did nothing because not eof!";
            qDebug()<<"socket size:"<<socket->getTotalBytes()<<"current size:"<<socket->getBytesHaveRead();
            return;
        }
    }
    else if(socket->bytesAvailable() && !socket->isNewSocket() )
    {
        qDebug()<<"socket size:"<<socket->getTotalBytes()<<"current Size:"<<socket->getBytesHaveRead();
        QByteArray inCommingContent=socket->readAll();
        socket->appendData(inCommingContent);

        if(socket->isEof())
        {
            qDebug()<<"eof! socket size:"<<socket->getTotalBytes()<<"current Size:"<<socket->getBytesHaveRead();

            PathTreeNode::TaskHandlerType handlerType;

            if(parser.method==HTTP_GET)
            {
                socket->getHeader().setHttpMethod(HttpHeader::HTTP_GET);
                handlerType=PathTreeNode::GET;
            }
            else if(parser.method==HTTP_POST)
            {
                socket->getHeader().setHttpMethod(HttpHeader::HTTP_POST);
                handlerType=PathTreeNode::POST;
            }
            else
            {
                qDebug()<<"not get and post"<<socket->atEnd()<<socket->bytesAvailable()<<socket->ConnectedState;
                qDebug()<<inCommingContent;
                socket->close();
                return;
            }

            HttpRequest request;

            HttpResponse response;

            request.getHeader()=socket->getHeader();

            QString contentLengthString=socket->getHeader().getHeaderInfo("Content-Length");

            if(!contentLengthString.isEmpty())
            {

                QString contentTypeString=socket->getHeader().getHeaderInfo("Content-Type");

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
                        QMap<QString,QByteArray> formData;

                        bool success=parseFormData(contentType,socket->getBuffer(),formData);
                        if(success)
                        {
                            request.setFormData(formData);
                        }
                        else
                        {
                            socket->close();
                            return;
                        }
                    }
                    else if(contentTypeString=="application/octet-stream")
                    {
                        qDebug()<<"uploading things!!";
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

            FILE *file=fopen("xxx.pdf","wb");
            fwrite(socket->getBuffer().data(),socket->getBuffer().count(),1,file);
            fclose(file);

            QTextStream os(socket);
            os<<"{success:true}";
            qDebug()<<"before closeing";
            socket->close();
        }
        else
            return;


    }
}

void Worker::discardClient()
{
    TcpSocket* socket = (TcpSocket*)sender();
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
