#include "pagetemplate.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>


PageTemplate::PageTemplate():QObject(),
    placeMarkers(),
    templateString(),
    placeMarkerRegExp("\\{\\{\\{([a-zA-Z0-9_-]+)\\}\\}\\}")
{

}

PageTemplate::PageTemplate(const QString &filename):QObject(),
    placeMarkers(),
    templateString(),
    placeMarkerRegExp("\\{\\{\\{([a-zA-Z0-9_-]+)\\}\\}\\}")
{
    loadFromFile(filename);
}

PageTemplate::PageTemplate(const PageTemplate &in):QObject(),
    placeMarkers(in.placeMarkers),
    templateString(in.templateString),
    placeMarkerRegExp("\\{\\{\\{([a-zA-Z0-9_-]+)\\}\\}\\}")
{
}

void PageTemplate::operator=(const PageTemplate &in)
{
    placeMarkers.clear();
    placeMarkers=in.placeMarkers;
    templateString=in.templateString;
}

PageTemplate::~PageTemplate()
{
}

void PageTemplate::setTemplate(const QString &temp)
{
    placeMarkers.clear();
    templateString.clear();
    templateString.append(temp);

    parseTemplate();
}

void PageTemplate::loadFromFile(const QString &filename)
{
    placeMarkers.clear();
    templateString.clear();

    QFile data(filename);
    if (data.open(QFile::ReadOnly ))
    {
        QDataStream in(&data);
        char *filecon=new char[data.size()];
        in.readRawData(filecon,data.size());

        templateString=QString().fromUtf8(filecon,data.size());

        delete [] filecon;
        data.close();

        parseTemplate();
    }
}

QByteArray PageTemplate::render()
{
    QString result=templateString;


    QMapIterator<QString, PlaceMarker> i(placeMarkers);
    while (i.hasNext())
    {
        i.next();

        for(int e=0;e<i.value().getPositions().count();++e)
            result.replace(i.value().getPositions()[e],i.value().getValue());
    }

    return result.toUtf8();
}

void PageTemplate::registerPlaceMarkerValue(const QString &field,const QByteArray &value)
{
    if(placeMarkers.contains(field))
    {
        placeMarkers[field].setValue(value);
    }
}

void PageTemplate::parseTemplate()
{
    int firstIndex=placeMarkerRegExp.indexIn(templateString);

    if(firstIndex)
    {
        QStringList markerList = placeMarkerRegExp.capturedTexts();
        int numberOfMarkers=placeMarkerRegExp.captureCount();

        //qDebug()<<markerList.count()<<numberOfMarkers;

        for(int i=0;i<numberOfMarkers;++i)
        {
            //qDebug()<<placeMarkerRegExp.pos(i)<<markerList.at(i);

            QString field=markerList.at(i).mid(3,markerList.at(i).length()-6);

            if(placeMarkers.contains(field))
            {
                placeMarkers[field].addPosition(placeMarkerRegExp.pos(i));
            }
            else
            {
                placeMarkers[field]=PlaceMarker(field,placeMarkerRegExp.pos(i));
            }
        }
    }
}
