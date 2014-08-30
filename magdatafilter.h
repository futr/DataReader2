#ifndef MAGDATAFILTER_H
#define MAGDATAFILTER_H

#include "abstractdatafilter.h"
#include <QTextStream>
#include <QtEndian>


class MagDataFilter : public AbstractDataFilter
{
    Q_OBJECT
public:
    explicit MagDataFilter(QObject *parent = 0);

    bool parseAndSave(unsigned int step, QByteArray data);

private:
    void makeHeader();

signals:

public slots:

};

#endif // MAGDATAFILTER_H
