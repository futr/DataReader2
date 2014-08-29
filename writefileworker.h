#ifndef WRITEFILEWORKER_H
#define WRITEFILEWORKER_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include "device_id.h"
#include "abstractdatafilter.h"
#include <time.h>
#include <QMap>
#include <QtEndian>

#define LOG_SIGNATURE 0x3E

class WriteFileWorker : public QObject
{
    Q_OBJECT
public:
    explicit WriteFileWorker(QObject *parent = 0);

private:
    QFile *m_logFile;
    QMap<int, AbstractDataFilter *> m_filterMap;
    bool stopFlag;
    bool m_error;
    qint64 m_progress;
    bool m_running;


public:
    bool error( void );
    qint64 getProgress( void );
    bool isRunning( void );

signals:
    void errorOccurred( void );
    void finished( void );

public slots:
    void setParameter( QFile *logFile, QMap<int, AbstractDataFilter *> filterMap );
    void doSaveFile( void );
    void stopSave( void );

};

#endif // WRITEFILEWORKER_H
