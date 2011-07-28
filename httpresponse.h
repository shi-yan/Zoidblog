#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>

class TcpSocket;

class HttpResponse:public QObject
{
    Q_OBJECT

    //QTextStream outputStream;

    QByteArray buffer;

    TcpSocket *socket;

public:

    QString debugInfo;
    HttpResponse(TcpSocket *_socket=0);
    HttpResponse(const HttpResponse &in);
    void operator=(const HttpResponse &in);
    ~HttpResponse();


    void operator<<(const QString &info);

    QByteArray & getBuffer()
    {
        return buffer;
    }


};

#endif // HTTPRESPONSE_H
