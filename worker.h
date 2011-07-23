#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "http_parser.h"

class Worker:public QThread
{
    Q_OBJECT
    QString workerName;
    http_parser parser;

public:
    void run();
    Worker(const QString _name);

public slots:
    void readClient();
    void discardClient();
};

#endif // WORKER_H
