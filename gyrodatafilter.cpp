#include "gyrodatafilter.h"

GyroDataFilter::GyroDataFilter(QObject *parent) :
    AbstractDataFilter(parent)
{
    m_fileName   = "gyro.csv";
    m_filterName = "GyroFilter";
    m_filterID   = ID_MPU9150_GYRO;
}

bool GyroDataFilter::parseAndSave(unsigned int step, QByteArray data)
{
    // データー処理
    qint16 x, y, z;
    QTextStream out( &m_file );

    x = qFromLittleEndian<qint16>( (uchar *)data.data() );
    y = qFromLittleEndian<qint16>( (uchar *)data.data() + 2 );
    z = qFromLittleEndian<qint16>( (uchar *)data.data() + 4 );

    out << step << ","
        << QString().sprintf( "%.6e", (double)x * 0.01526 ) << ","
        << QString().sprintf( "%.6e", (double)y * 0.01526 ) << ","
        << QString().sprintf( "%.6e", (double)z * 0.01526 ) << "\r\n";

    return true;
}

void GyroDataFilter::makeHeader()
{
    // ヘッダ作成
    QTextStream out( &m_file );

    out << "time[100us],x[dps],y[dps],z[dps]\r\n";
}
