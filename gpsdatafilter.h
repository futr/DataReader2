#ifndef GPSDATAFILTER_H
#define GPSDATAFILTER_H

#include "abstractdatafilter.h"

class GPSDataFilter : public AbstractDataFilter
{
    Q_OBJECT
public:
    explicit GPSDataFilter(QObject *parent = 0);

    bool parseAndSave(unsigned int step, QByteArray data);

signals:

public slots:

};

#endif // GPSDATAFILTER_H
