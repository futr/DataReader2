#include "magdatafilter.h"

MagDataFilter::MagDataFilter(QObject *parent) :
    AbstractDataFilter(parent)
{
    m_fileName   = "mag.csv";
    m_filterName = "MagFilter";
    m_filterID   = ID_AK8975;
}

bool MagDataFilter::parseAndSave(unsigned int step, QByteArray data)
{
    // データー処理
    qint16 x, y, z;
    QTextStream out( &m_file );

    x = qFromLittleEndian<qint16>( (uchar *)data.data() );
    y = qFromLittleEndian<qint16>( (uchar *)data.data() + 2 );
    z = qFromLittleEndian<qint16>( (uchar *)data.data() + 4 );

    out << step << ","
        << QString().sprintf( "%.6e", (double)x * 0.3 ) << ","
        << QString().sprintf( "%.6e", (double)y * 0.3 ) << ","
        << QString().sprintf( "%.6e", (double)z * 0.3 ) << "\r\n";

    return true;
}

void MagDataFilter::makeHeader()
{
    // ヘッダ作成
    QTextStream out( &m_file );

    out << "time[100us],x[uT],y[uT],z[uT]\r\n";
}
