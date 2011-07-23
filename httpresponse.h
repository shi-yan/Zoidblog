#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>

class HttpResponse:public QObject
{
    Q_OBJECT

    //QTextStream outputStream;

    QByteArray buffer;



public:

    QString debugInfo;
    HttpResponse();
    HttpResponse(const HttpResponse &in);
    void operator=(const HttpResponse &in);
    ~HttpResponse();


    void operator<<(const QString &info);


};

#endif // HTTPRESPONSE_H
