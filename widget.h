#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>

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
};

#endif // WIDGET_H
