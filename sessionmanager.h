#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "session.h"
#include <QMutex>

class SessionManager:public QObject
{
    Q_OBJECT

    QMap<QString,Session> sessions;


    QMutex sessionMutex;

    int checkerCount;

    bool needToAdd;

    int maximumSessionSize;

    int expirationTimeInSeconds;

     SessionManager(QObject *parent=0);
     ~SessionManager();

    QString generateKey();



public:

    static SessionManager & getSingleton()
    {
        static SessionManager obj;
        return obj;
    }

    void setExpirationTime(int sec)
    {
        expirationTimeInSeconds=sec;
    }

    void setMaximumSessions(int size)
    {
        maximumSessionSize=size;
    }

    const QString addSession(const QVariant userData);

    const QVariant checkSession(const QString &_key);

};

#endif // SESSIONMANAGER_H
