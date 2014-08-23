#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
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
    // Open a log file
    logFileName = QFileDialog::getOpenFileName( this, "Select log file" );

    // Check the log file name
    if ( logFileName == "" ) {
        return;
    }
}

void Widget::on_saveButton_clicked()
{
    // Select the dir to save and save the log
    QString saveDir;

    // Get dir to save
    saveDir = QFileDialog::getExistingDirectory( this, "Select a dir to save" );

    // Check
    if ( saveDir == "" ) {
        return;
    }
}
