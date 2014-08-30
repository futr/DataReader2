#ifndef GYRODATAFILTER_H
#define GYRODATAFILTER_H

#include "abstractdatafilter.h"
#include <QTextStream>
#include <QtEndian>

class GyroDataFilter : public AbstractDataFilter
{
    Q_OBJECT
public:
    explicit GyroDataFilter(QObject *parent = 0);

    bool parseAndSave(unsigned int step, QByteArray data);

private:
    void makeHeader();

signals:

public slots:

};

#endif // GYRODATAFILTER_H
