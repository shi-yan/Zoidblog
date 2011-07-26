#include "sessionmanager.h"
#include <QDateTime>
#include <cstdlib>
#include <QThread>
#include <QVector>
#include <QtCore/QCoreApplication>

 SessionManager::SessionManager(QObject *parent):QObject(parent),checkerCount(0),needToAdd(false),maximumSessionSize(100),expirationTimeInSeconds(3600)
{
     srand(QDateTime::currentDateTime().toTime_t());
}

 SessionManager::~SessionManager()
{

}


const QString SessionManager::addSession(const QVariant userData)
{
    bool successfullyAddedUserData=false;

    QString key;
    needToAdd=true;

    while(successfullyAddedUserData==false)
    {
        key=generateKey();
        sessionMutex.lock();
        while(checkerCount>0)
        {
            sessionMutex.unlock();


            QCoreApplication::processEvents();
            sessionMutex.lock();
        }

        if(!sessions.contains(key))
        {
            if(sessions.size()>=maximumSessionSize)
            {
                QVector<QString> toBeRemoved;
                toBeRemoved.reserve(100);

                QString oldestSessionKey;
                int oldestSessionLife=0;
                QList<Session> values = sessions.values();
                for (int i = 0; i < values.size(); ++i)
                {
                    Session olds=values.at(i);

                    QDateTime now=QDateTime::currentDateTime();

                    int elapsedTime = olds.getSessionTime().secsTo(now);

                    if(elapsedTime>expirationTimeInSeconds)
                    {
                        toBeRemoved.push_back(olds.getSessionKey());
                    }
                    else
                    {
                        if(oldestSessionLife<elapsedTime)
                        {
                            oldestSessionKey=olds.getSessionKey();
                            oldestSessionLife=elapsedTime;
                        }
                    }
                }


                if(toBeRemoved.count()==0)
                {
                    sessions.remove(oldestSessionKey);
                }
                else
                {
                    for(int i=0;i<toBeRemoved.count();++i)
                    {
                        sessions.remove(toBeRemoved[i]);
                    }
                }
            }

            Session s(key,userData);
            sessions[key]=s;
            successfullyAddedUserData=true;
            needToAdd=false;
        }

        sessionMutex.unlock();
    }

    return key;

}

const QVariant SessionManager::checkSession(const QString &_key)
{
    sessionMutex.lock();

    while(needToAdd)
    {
        sessionMutex.unlock();

        QCoreApplication::processEvents();

        sessionMutex.lock();
    }

    checkerCount++;

    sessionMutex.unlock();


    Session s= sessions[_key];

    if(!s.getSessionKey().isNull())
    {
        s.setSessionTime(QDateTime::currentDateTime());
    }

    sessionMutex.lock();

    checkerCount--;

    sessionMutex.unlock();

    return s.getUserData();
}

QString SessionManager::generateKey()
{
    static unsigned char sessionIDCharSet[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
                                            ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                                             '-','_','~','0','1','2','3','4','5','6','7','8','9'};


    char sessionKey[65];

    for(int i=0;i<64;++i)
    {
        sessionKey[i]=sessionIDCharSet[rand()%65];
    }

    sessionKey[64]=0;


    QString result(sessionKey);


    return result;
}
