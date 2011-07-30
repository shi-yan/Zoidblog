#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "http_parser.h"
#include <QMap>
#include "pathtree.h"
#include "webapp.h"

class Worker:public QThread
{
    Q_OBJECT
    QString workerName;
    http_parser parser;

    bool inHandlingARequest;

    QMap<int,WebApp*> webAppTable;

    PathTree pathTree;

    bool parseFormData(const QString &contentTypeString,const QByteArray &_body,QMap<QString,QByteArray> &formData);

public:
    void run();
    Worker(const QString _name);

    void registerWebApps(QVector<int> &webAppClassIDs);


public slots:
    void readClient();
    void discardClient();

    void newSocket(int socketid);
};

#endif // WORKER_H
