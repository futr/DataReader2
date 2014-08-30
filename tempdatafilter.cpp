#include "tempdatafilter.h"

TempDataFilter::TempDataFilter(QObject *parent) :
    AbstractDataFilter(parent)
{
    m_fileName   = "temperature.csv";
    m_filterName = "TempFilter";
    m_filterID   = ID_MPU9150_TEMP;
}

bool TempDataFilter::parseAndSave(unsigned int step, QByteArray data)
{
    // データー処理
    qint16 val;
    QTextStream out( &m_file );

    val = qFromLittleEndian<qint16>( (uchar *)data.data() );

    out << step << ","
        << QString().sprintf( "%.6e", (double)val / 340 + 35 ) << "\r\n";

    return true;
}

void TempDataFilter::makeHeader()
{
    // ヘッダ作成
    QTextStream out( &m_file );

    out << "time[100us],temperature[C]\r\n";

}
