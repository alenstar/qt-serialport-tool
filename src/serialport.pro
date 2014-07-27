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

SOURCES += main.cpp\
        mainwindow.cpp \
    qled.cpp \
    setupdialog.cpp \
    execdialog.cpp

HEADERS  += mainwindow.h \
    qled.h \
    setupdialog.h \
    execdialog.h

FORMS    += mainwindow.ui \
    setupdialog.ui \
    execdialog.ui

RESOURCES += \
    mainwindow.qrc
