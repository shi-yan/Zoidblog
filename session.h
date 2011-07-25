#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVariant>

class Session:public QObject
{
    Q_OBJECT

    QString sessionKey;
    QVariant userData;
    QDateTime sessionTime;

public:
    Session(QObject *parent=0);
    Session(const QString _sessionKey,const QVariant _userID,QObject *parent=0);
    Session(const Session &in);
    void operator=(const Session &in);

    const QString & getSessionKey()
    {
        return sessionKey;
    }

    const QVariant & getUserData()
    {
        return userData;
    }

    void setSessionTime(const QDateTime &now)
    {
        sessionTime=now;
    }

    const QDateTime & getSessionTime()
    {
        return sessionTime;
    }
};

#endif // SESSION_H
