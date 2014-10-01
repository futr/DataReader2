#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QThread>
#include <time.h>
#include <QFileInfo>
#include "progressdialog.h"
#include "writefileworker.h"
#include "abstractdatafilter.h"
#include "gpsdatafilter.h"
#include "accdatafilter.h"
#include "gyrodatafilter.h"
#include "magdatafilter.h"
#include "tempdatafilter.h"
#include "pressdatafilter.h"
#include "device_id.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_closeButton_clicked();

    void on_openButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::Widget *ui;

    QString logFileName;
    QFile logFile;

    void disableUI();
};

#endif // WIDGET_H
