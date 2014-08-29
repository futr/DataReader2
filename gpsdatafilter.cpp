#include "gpsdatafilter.h"

GPSDataFilter::GPSDataFilter(QObject *parent) :
    AbstractDataFilter(parent)
{
    m_fileName   = "gps.nmea";
    m_filterName = "GPSFilter";
    m_filterID   = ID_GPS;
}

bool GPSDataFilter::parseAndSave(unsigned int step, QByteArray data)
{
    // データーを処理して保存

    // GPSの場合はそのままそのまま保存すれば良い
    m_file.write( data );

    return true;
}
