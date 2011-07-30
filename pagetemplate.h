#ifndef PAGETEMPLATE_H
#define PAGETEMPLATE_H

#include <QObject>
#include <QMap>
#include "placemarker.h"
#include <QRegExp>

class PageTemplate:public QObject
{
    Q_OBJECT

    QMap<QString,PlaceMarker> placeMarkers;

    QString templateString;

    void parseTemplate();

     QRegExp placeMarkerRegExp;

public:
    PageTemplate();
    PageTemplate(const QString &filename);

    PageTemplate(const PageTemplate &);
    void operator=(const PageTemplate &);

    ~PageTemplate();

    void setTemplate(const QString &temp);
    void loadFromFile(const QString &filename);

    QByteArray render();
    void registerPlaceMarkerValue(const QString &field,const QByteArray &value);

};

#endif // PAGETEMPLATE_H
