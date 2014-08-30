#ifndef ACCDATAFILTER_H
#define ACCDATAFILTER_H

#include "abstractdatafilter.h"
#include <QTextStream>
#include <QtEndian>

class AccDataFilter : public AbstractDataFilter
{
    Q_OBJECT
public:
    explicit AccDataFilter(QObject *parent = 0);

    bool parseAndSave(unsigned int step, QByteArray data);

private:
    void makeHeader();

signals:

public slots:

};

#endif // ACCDATAFILTER_H
