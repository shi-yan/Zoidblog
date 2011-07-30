#ifndef PLACEMARKER_H
#define PLACEMARKER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QVector>

class PlaceMarker:public QObject
{
    Q_OBJECT

    QVector<int> positions;

    QString fieldName;

    QString value;
public:
    PlaceMarker();
    PlaceMarker(const QString &_fieldName,int pos);
    PlaceMarker(const PlaceMarker &in);
    void operator=(const PlaceMarker &in);

    const QVector<int> & getPositions() const
    {
        return positions;
    }

    void addPosition(int pos)
    {
        positions.push_back(pos);
    }

    unsigned int getSize()
    {
        return fieldName.count()+6;
    }

    const QString & getValue() const
    {
        return value;
    }

    void setValue(const QString &in)
    {
        value=in;
    }

    bool isNull()
    {
        return fieldName.isNull() || fieldName.isEmpty();
    }

};

#endif // PLACEMARKER_H
