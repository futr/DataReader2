#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T22:38:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataReader2
TEMPLATE = app

TRANSLATIONS = dr2_ja.ts

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

# Use c++11 for lambda and auto
QMAKE_CXXFLAGS += -std=c++11

win32 {
    RC_FILE = win.rc
}

OTHER_FILES += \
    win.rc
