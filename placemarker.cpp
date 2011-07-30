#include "placemarker.h"

PlaceMarker::PlaceMarker():QObject(),
    positions(),fieldName(),value()
{

}

PlaceMarker::PlaceMarker(const QString &_fieldName,int pos):QObject(),
    positions(),fieldName(_fieldName),value()
{

}

PlaceMarker::PlaceMarker(const PlaceMarker &in):QObject(),
    positions(in.positions),fieldName(in.fieldName),value(in.value)
{

}

void PlaceMarker::operator=(const PlaceMarker &in)
{
    positions=in.positions;
    fieldName=in.fieldName;
    value=in.value;
}
