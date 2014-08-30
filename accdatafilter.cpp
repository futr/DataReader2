#include "accdatafilter.h"

AccDataFilter::AccDataFilter(QObject *parent) :
    AbstractDataFilter(parent)
{
    m_fileName   = "acc.csv";
    m_filterName = "AccFilter";
    m_filterID   = ID_MPU9150_ACC;
}

bool AccDataFilter::parseAndSave(unsigned int step, QByteArray data)
{
    // データー処理
    qint16 x, y, z;
    QTextStream out( &m_file );

    x = qFromLittleEndian<qint16>( (uchar *)data.data() );
    y = qFromLittleEndian<qint16>( (uchar *)data.data() + 2 );
    z = qFromLittleEndian<qint16>( (uchar *)data.data() + 4 );

    out << step << ","
        << QString().sprintf( "%.6e", (double)x * 0.122 / 1000 ) << ","
        << QString().sprintf( "%.6e", (double)y * 0.122 / 1000 ) << ","
        << QString().sprintf( "%.6e", (double)z * 0.122 / 1000 ) << "\r\n";

    return true;
}

void AccDataFilter::makeHeader()
{
    // ヘッダ作成
    QTextStream out( &m_file );

    out << "time[100us],x[G],y[G],z[G]\r\n";
}
