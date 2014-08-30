#include "pressdatafilter.h"

PressDataFilter::PressDataFilter(QObject *parent) :
    AbstractDataFilter(parent)
{
    m_fileName   = "pressure.csv";
    m_filterName = "PressFilter";
    m_filterID   = ID_LPS331AP;
}

bool PressDataFilter::parseAndSave(unsigned int step, QByteArray data)
{
    // データー処理
    qint32 val;
    QTextStream out( &m_file );

    val = qFromLittleEndian<qint32>( (uchar *)data.data() );

    out << step << ","
        << QString().sprintf( "%.6e", (double)val / 4096 ) << "\r\n";

    return true;
}

void PressDataFilter::makeHeader()
{
    // ヘッダ作成
    QTextStream out( &m_file );

    out << "time[100us],pressure[hPa]\r\n";
}
