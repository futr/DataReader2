#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T22:38:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataReader2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    abstractdatafilter.cpp \
    gpsdatafilter.cpp \
    progressdialog.cpp \
    writefileworker.cpp

HEADERS  += widget.h \
    abstractdatafilter.h \
    gpsdatafilter.h \
    device_id.h \
    progressdialog.h \
    writefileworker.h

FORMS    += widget.ui \
    progressdialog.ui

# autoやラムダを使っているので有効にしています.マイクロソフトのコンパイラーでは違うオプションが必要だと思います．
QMAKE_CXXFLAGS += -std=c++11
