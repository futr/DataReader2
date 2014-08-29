#include "writefileworker.h"

WriteFileWorker::WriteFileWorker(QObject *parent) :
    QObject(parent),
    m_running( false )
{
}

bool WriteFileWorker::error()
{
    // Error is occurred
    return m_error;
}

qint64 WriteFileWorker::getProgress()
{
    return m_progress;
}

bool WriteFileWorker::isRunning()
{
    // 走っているか
    return m_running;
}

void WriteFileWorker::setParameter( QFile *logFile, QMap<int, AbstractDataFilter *> filterMap )
{
    // Set the parameter
    m_logFile = logFile;
    m_filterMap = filterMap;
}

void WriteFileWorker::doSaveFile( void )
{
    // Save readFile to writeFile
    // init
    m_progress = 0;
    stopFlag   = false;
    m_error    = false;
    m_running  = true;

    // Init fp
    m_logFile->seek( 2 );

    // Save
    while ( !m_logFile->atEnd() ) {
        QByteArray header;
        QByteArray data;
        QByteArray signatureBuf;
        int signature;
        int devID;
        int dataSize;
        quint32 step;

        // シグネチャ読み込み
        signatureBuf = m_logFile->read( 1 );

        // サイズチェック
        if ( signatureBuf.size() < 1 ) {
            qWarning( "シグネチャ読み込み失敗" );

            m_error = true;

            break;
        }

        signature = (unsigned char)signatureBuf[0];

        // シグネチャチェック
        if ( signature == 0 ) {
            // シグネチャ == 0なので終了
            break;
        } else if ( signature != LOG_SIGNATURE ) {
            qWarning( "シグネチャが一致しない : sig(%d) id(%d) size(%d) pos(%d)", signature, devID, dataSize, (int)m_logFile->pos() );

            m_error = true;

            break;
        }

        // Read and parse header
        header = m_logFile->read( 6 );

        // サイズチェック
        if ( header.size() != 6 ) {
            m_error = true;

            break;
        }

        step      = qFromLittleEndian<quint32>( (uchar *)header.data() );
        devID     = (unsigned char)header[4];
        dataSize  = (unsigned char)header[5];

        // データー部分読み込み
        data = m_logFile->read( dataSize );

        // もしGPSデーターだった場合は，サイズが適切ではない可能性があるので次のシグネチャまで読む
        if ( devID == ID_GPS ) {
            while ( 1 ) {
                QByteArray buf = m_logFile->read( 1 );

                if ( buf.size() < 1 ) {
                    break;
                }

                // ファイル終端かシグネチャ発見
                if ( (unsigned char)buf[0] == LOG_SIGNATURE || (unsigned char)buf[0] == 0 ) {
                    m_logFile->seek( m_logFile->pos() - 1 );

                    break;
                }

                data += buf;
            };

            if ( dataSize != data.size() ) {
                qWarning( "GPSデーターオーバーフロー : %d", data.size() );

                dataSize = data.size();
            }
        }

        // 読み込めなければ失敗
        if ( data.size() != dataSize ) {
            qWarning( "データーが足りない" );

            m_error = true;

            break;
        }

        // フィルター適用
        AbstractDataFilter *filter;

        filter = m_filterMap[devID];

        // 対応するフィルターがなかった
        if ( 0 == filter ) {
            // qWarning( "There was no match filter : %d", devID );

            continue;
        }

        // フィルター処理
        filter->parseAndSave( step, data );

        // 進捗設定
        m_progress = m_logFile->pos() / 1024;
    }

    // emit error
    if ( m_error ) {
        emit errorOccurred();
    }

    emit finished();

    m_running = false;
}

void WriteFileWorker::stopSave()
{
    // Stop saving
    stopFlag = true;
}
