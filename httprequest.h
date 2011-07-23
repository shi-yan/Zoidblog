#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include "httpheader.h"

class HttpRequest:public QObject
{
    Q_OBJECT

    HttpHeader header;
public:

    QString debugInfo;

    HttpRequest();
    HttpRequest(const HttpRequest &in);
    void operator=(const HttpRequest &in);

    HttpHeader & getHeader()
    {
        return header;
    }

    ~HttpRequest();
};

#endif // HTTPREQUEST_H
