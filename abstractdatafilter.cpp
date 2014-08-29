#include "abstractdatafilter.h"

AbstractDataFilter::AbstractDataFilter(QObject *parent) :
    QObject(parent),
    m_fileName("default.csv"),
    m_filterName("DefaultFilter")
{
}

AbstractDataFilter::~AbstractDataFilter()
{
    // デストラクタ－起動時にファイルが開かれていれば閉じる
    if ( m_file.isOpen() ) {
        m_file.close();
    }
}

QString AbstractDataFilter::getFileName()
{
    // ファイル名を返す
    return m_fileName;
}

void AbstractDataFilter::setFileName(QString name)
{
    // ファイル名を設定
    m_fileName = name;
}

bool AbstractDataFilter::openFile(QString path)
{
    // ファイルを開く
    m_file.setFileName( path + "/" + m_fileName );

    return m_file.open( QFile::WriteOnly );
}

void AbstractDataFilter::closeFile()
{
    // ファイルを閉じる
    m_file.close();
}

int AbstractDataFilter::getFilterID()
{
    // フィルターIDを返す
    return m_filterID;
}
