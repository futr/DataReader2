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
    logFileName = QFileDialog::getOpenFileName( this, "ログファイルを選択してください" );

    // Check the log file name
    if ( logFileName == "" ) {
        return;
    }

    // ログファイル開く
    logFile.close();

    logFile.setFileName( logFileName );

    if ( !logFile.open( QFile::ReadOnly ) ) {
        QMessageBox::critical( this, "失敗", "ログファイルのオープンに失敗しました" );

        return;
    }

    // ログファイルかどうか確認
    QByteArray header;

    header = logFile.read( 2 );

    if ( (unsigned char)header[0] != DEVICE_LOG_SIGNATURE ) {
        // シグネチャが一致しないので失敗
        QMessageBox::critical( this, "失敗", QString( "正常なログファイルではありません\n%1" ).arg( (int)header[0] ) );

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
    time_t beforeTime;
    qint64 currentProgress;
    qint64 beforeProgress;
    QMessageBox warningBox;

    // Create the warning box
    warningBox.setWindowTitle( "失敗" );
    warningBox.setText( "ファイルの書き出しに失敗しました." );
    warningBox.setInformativeText( "書き出し中に何らかの問題が発生しました．\n正常にファイルが保存できていない可能性があります．" );
    warningBox.setIcon( QMessageBox::Warning );
    warningBox.setStandardButtons( QMessageBox::Ok );

    // Get dir to save
    saveDir = QFileDialog::getExistingDirectory( this, "保存先を選択してください" );

    // Check
    if ( saveDir == "" ) {
        return;
    }

    // 保存開始

    // 保存用クラス実体化
    ProgressDialog *progress = new ProgressDialog();
    QThread *writeFileThread = new QThread();
    WriteFileWorker *worker  = new WriteFileWorker();

    // フィルター実体化
    QMap<int, AbstractDataFilter *> filterMap;

    if ( ui->gpsCheckBox->isChecked() ) {
        filterMap[ID_GPS] = new GPSDataFilter();
    }

    // (実験)後始末関数をラムダで作ってみる
    auto deleteAll = [&] {
        foreach ( AbstractDataFilter *filter , filterMap ) {
            delete filter;
        }

        delete progress;
        delete worker;
        delete writeFileThread;
    };

    // フィルター設定
    bool opened = true;

    foreach ( AbstractDataFilter *filter , filterMap ) {
        if ( !filter->openFile( saveDir ) ) {
            opened = false;
        }
    }

    // オープンエラーチェック
    if ( !opened ) {
        QMessageBox::critical( this, "失敗", "ファイルが開けませんでした" );

        deleteAll();

        return;
    }

    // ワーカー設定
    worker->moveToThread( writeFileThread );
    worker->setParameter( &logFile, filterMap );

    // シグナル接続
    connect( worker,          SIGNAL(finished()),    progress, SLOT(accept()) );
    connect( progress,        SIGNAL(finished(int)), worker,   SLOT(stopSave()), Qt::DirectConnection );
    connect( writeFileThread, SIGNAL(started()),     worker,   SLOT(doSaveFile()) );

    // 開始
    writeFileThread->start();

    beforeTime     = time( NULL );
    beforeProgress = 0;

    // Open dialog
    progress->show();

    // Event loop
    while ( 1 ) {
        if ( !worker->isRunning() ) {
            break;
        }

        // Update progress
        if ( beforeTime != time( NULL ) ) {
            beforeTime = time( NULL );

            currentProgress = worker->getProgress();

            progress->setProgressPos( currentProgress, logFile.size() / 1024, ( currentProgress - beforeProgress ) * 1024, saveDir );

            beforeProgress = currentProgress;
        }

        QApplication::processEvents();
    }

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

    ui->label->setText( "ファイル名" );
}
