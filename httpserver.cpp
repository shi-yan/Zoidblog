#include "httpserver.h"

int header_value_cbdd (http_parser *parser, const char *p,size_t len)
{
    //assert(parser);

    char *buffer=new char[len+1];

    strncpy(buffer,p,len);

    buffer[len]=0;

    qDebug()<<"==========";
    qDebug()<<buffer;
    qDebug()<<"==========";

    delete [] buffer;
    return 0;
}

void HttpServer::readClient()
{
    if (disabled)
        return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {

     /*   QString firstLine;

        QStringList tokens = (firstLine=QString(socket->readLine())).split(QRegExp("[ \r\n][ \r\n]*"));

        qDebug()<<firstLine;

        if (tokens[0] == "GET")
        {




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

                qDebug()<<"closed by read function";
            }
            else
                qDebug()<<"socket state:"<<socket->state();

      */





        QByteArray inCommingContent=socket->readAll();




        http_parser_settings settings;

        settings.on_message_begin=0;
        settings. on_path=0;
        settings. on_query_string=0;
        settings. on_url=0;
        settings. on_fragment=0;
        settings. on_header_field=header_value_cbdd;
        settings. on_header_value=header_value_cbdd;
        settings.      on_headers_complete=0;
        settings. on_body=0;
        settings.      on_message_complete=0;

        http_parser *parser=(http_parser*)malloc(sizeof(http_parser));

        http_parser_init(parser,HTTP_REQUEST);
        parser->data = socket;

        qDebug()<<"before execution. Buffer size:"<<inCommingContent.count();

        size_t nparsed = http_parser_execute(parser,&settings,inCommingContent.constData(),inCommingContent.count());

        qDebug()<<"finish execution";

        if(parser->upgrade)
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

            qDebug()<<parser->method;
        }


        free(parser);












        QTextStream os(socket);

        os << "HTTP/1.0 200 Ok\r\n"
            "Content-Type: text/html; charset=\"utf-8\"\r\n"
            "\r\n"
            "<h1>Nothing to see here</h1>\n"
            << QDateTime::currentDateTime().toString() << "\n"
            <<"received:<br/>"
              <<inCommingContent;

        socket->close();



        }
    }
