#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "http_parser.h"
#include <QMap>

class Worker:public QThread
{
    Q_OBJECT
    QString workerName;
    http_parser parser;

    bool parseFormData(const QString &contentTypeString,const QByteArray &_body,QMap<QString,QByteArray> &formData);

public:
    void run();
    Worker(const QString _name);



public slots:
    void readClient();
    void discardClient();

    void newSocket(int socketid);
};

#endif // WORKER_H
