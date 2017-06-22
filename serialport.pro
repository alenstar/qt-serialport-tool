#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T12:07:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialport
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -D_GLIBCXX_USE_CXX11_ABI

include(./qextserialport/qextserialport.pri)

INCLUDEPATH += src ./ include/
SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/qled.cpp \
    src/setupdialog.cpp \
    src/execdialog.cpp \
    src/mdialog.cpp \
    src/aerobutton/aerobutton.cpp \
    src/helper.cpp \
    src/messagebox.cpp \
    src/aeio/adlist.c \
    src/aeio/ae.c \
    src/aeio/anet.c \
    src/aeio/common.c \
    src/aeio/forward.c \
    src/aeio/syncio.c \
    src/aeio/zmalloc.c \
    src/tcp/tcpclient.cpp \
    src/udp/udpclient.cpp \
    src/tcp/tcpserver.cpp \
    src/udp/udpserver.cpp \
    src/loop/loop.cpp

HEADERS  += src/mainwindow.h \
    src/qled.h \
    src/setupdialog.h \
    src/execdialog.h \
    src/mdialog.h \
    src/aerobutton/aerobutton.h \
    src/helper.h \
    src/messagebox.h \
    src/tcp/tcpclient.h \
    src/udp/udpclient.h \
    src/tcp/tcpserver.h \
    src/udp/udpserver.h \
    src/loop/loop.h

FORMS    += ui/mainwindow.ui \
    ui/setupdialog.ui \
    ui/execdialog.ui

RESOURCES += \
    res/mainwindow.qrc
