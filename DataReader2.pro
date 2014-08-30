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
    writefileworker.cpp \
    accdatafilter.cpp \
    gyrodatafilter.cpp \
    magdatafilter.cpp \
    pressdatafilter.cpp \
    tempdatafilter.cpp

HEADERS  += widget.h \
    abstractdatafilter.h \
    gpsdatafilter.h \
    device_id.h \
    progressdialog.h \
    writefileworker.h \
    accdatafilter.h \
    gyrodatafilter.h \
    magdatafilter.h \
    pressdatafilter.h \
    tempdatafilter.h

FORMS    += widget.ui \
    progressdialog.ui

# autoã?©ã?ã?ãä½¿ã£ã¦ã?ã®ã§æå¹ã«ãã¦ã?¾ã?.ãã¤ã¯ã­ã½ããã®ã³ã³ãã¤ã©ã¼ã§ã¯éããªãã·ã§ã³ãå¿?¦ã ã¨æãã¾ãï¼?
QMAKE_CXXFLAGS += -std=c++11
