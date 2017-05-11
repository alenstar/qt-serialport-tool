#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T12:07:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialport
TEMPLATE = app

include(./qextserialport/qextserialport.pri)

INCLUDEPATH += src ./
SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/qled.cpp \
    src/setupdialog.cpp \
    src/execdialog.cpp

HEADERS  += src/mainwindow.h \
    src/qled.h \
    src/setupdialog.h \
    src/execdialog.h

FORMS    += ui/mainwindow.ui \
    ui/setupdialog.ui \
    ui/execdialog.ui

RESOURCES += \
    res/mainwindow.qrc
