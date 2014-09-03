#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // UI無効
    disableUI();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_closeButton_clicked()
{
    close();
}

void Widget::on_openButton_clicked()
{
    // ログファイル開く

    // UI無効
    disableUI();

    // ファイル名取得
    logFileName = QFileDialog::getOpenFileName( this, tr( "Select a log file" ) );

    // Check the log file name
    if ( logFileName == "" ) {
        return;
    }

    // ログファイル開く
    logFile.close();

    logFile.setFileName( logFileName );

    if ( !logFile.open( QFile::ReadOnly ) ) {
        QMessageBox::critical( this, tr( "Error" ), tr( "Can't open a log file" ) );

        return;
    }

    // ログファイルかどうか確認
    QByteArray header;

    header = logFile.read( 2 );

    if ( (unsigned char)header[0] != DEVICE_LOG_SIGNATURE ) {
        // シグネチャが一致しないので失敗
        QMessageBox::critical( this, tr( "Error" ), QString( tr( "Invalid log file\n%1" ) ).arg( (int)header[0] ) );

        logFile.close();

        return;
    }

    // UIを設定
    ui->label->setText( logFileName );
    ui->saveButton->setEnabled( true );

    // 有効なデバイスに対応するチェックボックス有効
    if ( header[1] & DEV_ACC ) {
        ui->accCheckBox->setEnabled( true );
    }

    if ( header[1] & DEV_GPS ) {
        ui->gpsCheckBox->setEnabled( true );
    }

    if ( header[1] & DEV_GYRO ) {
        ui->gyroCheckBox->setEnabled( true );
    }

    if ( header[1] & DEV_MAG ) {
        ui->magCheckBox->setEnabled( true );
    }

    if ( header[1] & DEV_PRESS ) {
        ui->pressCheckBox->setEnabled( true );
    }

    if ( header[1] & DEV_TEMP ) {
        ui->tempCheckBox->setEnabled( true );
    }
}

void Widget::on_saveButton_clicked()
{
    // Select the dir to save and save the log
    QString saveDir;
    QMessageBox warningBox;

    // Create the warning box
    warningBox.setWindowTitle( tr( "Warning" ) );
    warningBox.setText( tr( "File write failed" ) );
    warningBox.setInformativeText( tr( "A problem has occurred on current operation." ) );
    warningBox.setIcon( QMessageBox::Warning );
    warningBox.setStandardButtons( QMessageBox::Ok );

    // Get dir to save
    saveDir = QFileDialog::getExistingDirectory( this, tr( "Select a dir to save" ) );

    // Check
    if ( saveDir == "" ) {
        return;
    }

    // 保存開始

    // 先頭へ
    logFile.seek( 2 );

    // 保存用クラス実体化
    ProgressDialog *progress = new ProgressDialog();
    QThread *writeFileThread = new QThread();
    WriteFileWorker *worker  = new WriteFileWorker();

    // フィルター登録
    QMap<int, QList<AbstractDataFilter *> > filterMap;

    if ( ui->gpsCheckBox->isEnabled() && ui->gpsCheckBox->isChecked() ) {
        filterMap[ID_GPS] = QList<AbstractDataFilter *>() << new GPSDataFilter();
    }

    if ( ui->accCheckBox->isEnabled() && ui->accCheckBox->isChecked() ) {
        filterMap[ID_MPU9150_ACC] = QList<AbstractDataFilter *>() << new AccDataFilter();
    }

    if ( ui->gyroCheckBox->isEnabled() && ui->gyroCheckBox->isChecked() ) {
        filterMap[ID_MPU9150_GYRO] = QList<AbstractDataFilter *>() << new GyroDataFilter();
    }

    if ( ui->magCheckBox->isEnabled() && ui->magCheckBox->isChecked() ) {
        filterMap[ID_AK8975] = QList<AbstractDataFilter *>() << new MagDataFilter();
    }

    if ( ui->tempCheckBox->isEnabled() && ui->tempCheckBox->isChecked() ) {
        filterMap[ID_MPU9150_TEMP] = QList<AbstractDataFilter *>() << new TempDataFilter();
    }

    if ( ui->pressCheckBox->isEnabled() && ui->pressCheckBox->isChecked() ) {
        filterMap[ID_LPS331AP] = QList<AbstractDataFilter *>() << new PressDataFilter();
    }

    // (実験)後始末関数をラムダで作ってみる
    auto deleteAll = [&] {
        foreach ( QList<AbstractDataFilter *> filterList, filterMap ) {
            foreach ( AbstractDataFilter *filter,  filterList ) {
                delete filter;
            }
        }

        delete progress;
        delete worker;
        delete writeFileThread;
    };

    // フィルター設定
    bool opened = true;

    foreach ( QList<AbstractDataFilter *> filterList, filterMap ) {
        foreach ( AbstractDataFilter *filter,  filterList ) {
            if ( !filter->openFile( saveDir ) ) {
                opened = false;
            }
        }
    }

    // オープンエラーチェック
    if ( !opened ) {
        QMessageBox::critical( this, ( "Error" ), tr( "Can't open a file to save" ) );

        deleteAll();

        return;
    }

    // ワーカー設定
    worker->moveToThread( writeFileThread );
    worker->setParameter( &logFile, filterMap );
    worker->setup();

    // シグナル接続
    connect( worker,          SIGNAL(finished()),    progress, SLOT(accept()) );
    connect( progress,        SIGNAL(finished(int)), worker,   SLOT(stopSave()), Qt::DirectConnection );
    connect( writeFileThread, SIGNAL(started()),     worker,   SLOT(doSaveFile()) );
    connect( worker, SIGNAL(progress(int,int,int,QString)), progress, SLOT(setProgressPos(int,int,int,QString)), Qt::QueuedConnection );

    // 開始
    writeFileThread->start();

    // プログレスダイアログ表示
    progress->setWindowFlags( Qt::MSWindowsFixedSizeDialogHint | Qt::Dialog );
    progress->exec();

    // Wait Save Thread
    writeFileThread->quit();
    writeFileThread->wait();

    // Check a error
    if ( worker->error() ) {
        warningBox.exec();
    }

    // 破棄
    deleteAll();
}

void Widget::disableUI()
{
    // 保存に関するUIを無効に
    ui->saveButton->setEnabled( false );
    ui->accCheckBox->setEnabled( false );
    ui->magCheckBox->setEnabled( false );
    ui->gyroCheckBox->setEnabled( false );
    ui->gpsCheckBox->setEnabled( false );
    ui->tempCheckBox->setEnabled( false );
    ui->pressCheckBox->setEnabled( false );

    ui->label->setText( tr( "File name" ) );
}
