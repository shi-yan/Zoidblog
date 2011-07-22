#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QStringBuilder>


class HttpHeader:public QObject
{
    Q_OBJECT

    enum HttpMethod
    {
        HTTP_DELETE=0,
        HTTP_GET,
        HTTP_HEAD,
        HTTP_POST,
        HTTP_PUT,

        /* pathological */
        HTTP_CONNECT,
        HTTP_OPTIONS,
        HTTP_TRACE,

        /* webdav */
        HTTP_COPY,
        HTTP_LOCK,
        HTTP_MKCOL,
        HTTP_MOVE,
        HTTP_PROPFIND,
        HTTP_PROPPATCH,
        HTTP_UNLOCK,

        /* subversion */
        HTTP_REPORT,
        HTTP_MKACTIVITY,
        HTTP_CHECKOUT,
        HTTP_MERGE,

        /* upnp */
        HTTP_MSEARCH,
        HTTP_NOTIFY,
        HTTP_SUBSCRIBE,
        HTTP_UNSUBSCRIBE
    };

    enum HttpMethod httpMethod;
    QByteArray body;
    QMap<QString,QString> headerInfo;
    QString fragment;
    QString queryString;
    QString path;
    QString host;
    QString url;
    QString currentHeaderField;

public:
    HttpHeader();

    HttpHeader(const HttpHeader &in);

    void operator=(const HttpHeader &in);

    ~HttpHeader();

    void setHttpMethod(const enum HttpMethod _httpMethod)
    {
        httpMethod=_httpMethod;
    }

    enum HttpMethod getHttpMethod()
    {
        return httpMethod;
    }

    QByteArray &getBody()
    {
        return body;
    }

    void setBody(const QByteArray &_body)
    {
        body=_body;
    }

    void setCurrentHeaderField(const QString &_currentHeaderField)
    {
        currentHeaderField=_currentHeaderField;
    }

    QMap<QString,QString> &getHeaderInfo()
    {
        return headerInfo;
    }

    QString &getHeaderInfo(const QString & _headerField)
    {
        if(headerInfo.contains(_headerField))
        {
            return headerInfo[_headerField];
        }
        else
        {
            QString empty;
            return empty;
        }
    }

    void addHeaderInfo(const QString &_headerValue)
    {
        headerInfo[currentHeaderField]=_headerValue;
    }

    void setFragment(const QString &_fragment)
    {
        fragment=_fragment;
    }

    void setQueryString(const QString &_queryString)
    {
        queryString=_queryString;
    }

    void setPath(const QString &_path)
    {
        path=_path;
    }

    void setHost(const QString &_host)
    {
        host=_host;
    }

    void setUrl(const QString &_url)
    {
        url=_url;
    }

    QString toString()
    {
        QString stringBuilder;

        stringBuilder = "Url:"% url% "<br/>\r\n"
        % "Host:"% host% "<br/>\r\n"
        % "Path:"% path% "<br/>\r\n"
        % "Query String:"% queryString% "<br/>\r\n";

        for(int i=0;i<headerInfo.count();++i)
        stringBuilder= stringBuilder % headerInfo.keys()[i]% ":"% headerInfo.values()[i]% "<br/>\r\n";

        stringBuilder= stringBuilder % "Fragment:"% fragment% "<br/>\r\n"
                    % "Body:"% body% "<br/>\r\n";

        return stringBuilder;
    }
};

#endif // HTTPHEADER_H
