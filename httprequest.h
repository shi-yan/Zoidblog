#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include "httpheader.h"

class HttpRequest:public QObject
{
    Q_OBJECT

    HttpHeader header;

    QMap<QString,QByteArray> formData;

    bool hasSetFormData;
public:

    QString debugInfo;

    HttpRequest();
    HttpRequest(const HttpRequest &in);
    void operator=(const HttpRequest &in);

    void setFormData(const QMap<QString,QByteArray> &_formData)
    {
        formData=_formData;
        hasSetFormData=true;
    }

    HttpHeader & getHeader()
    {
        return header;
    }

    QByteArray & getFormData(const QString &fieldName)
    {
        return formData[fieldName];
    }

    bool hasFormData()
    {
        return hasSetFormData;
    }

    ~HttpRequest();
};

#endif // HTTPREQUEST_H
