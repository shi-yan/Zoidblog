#include "session.h"

Session::Session(QObject *parent):QObject(parent),sessionKey(),userData(),sessionTime()
{
    sessionTime=QDateTime::currentDateTime();
}

Session::Session(const QString _sessionKey,const QVariant _userData,QObject *parent)
    :QObject(parent),sessionKey(_sessionKey),userData(_userData),sessionTime()
{
    sessionTime=QDateTime::currentDateTime();
}

Session::Session(const Session &in)
    :QObject(),sessionKey(in.sessionKey),userData(in.userData),sessionTime(in.sessionTime)
{

}

void Session::operator=(const Session &in)
{
    sessionKey=in.sessionKey;
    sessionTime=in.sessionTime;
    userData=in.userData;
}
