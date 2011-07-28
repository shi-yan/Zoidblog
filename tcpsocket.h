#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include "httprequest.h"
#include "httpresponse.h"

class TcpSocket:public QTcpSocket
{
    Q_OBJECT

    bool isNew;

    HttpRequest request;
    HttpResponse response;

public:
    TcpSocket(QObject *parent=0);
    TcpSocket(const TcpSocket &in);
    void operator=(const TcpSocket &in);
    ~TcpSocket();

    void setRawHeader(const QString &in);
    QString & getRawHeader();

    unsigned int getTotalBytes();
    unsigned int getBytesHaveRead();
    HttpHeader & getHeader();
    void setHttpHeader(HttpHeader &_header);
    bool isEof();
    void notNew();

    bool isNewSocket();
    void setTotalBytes(unsigned int _totalBytes);

    void appendData(const char* buffer,unsigned int size);
    void appendData(const QByteArray &buffer);

    QByteArray &getBuffer();


};

#endif // TCPSOCKET_H
