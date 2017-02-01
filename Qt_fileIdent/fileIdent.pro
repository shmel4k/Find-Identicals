#-------------------------------------------------
#
# Project created by QtCreator 2017-01-31T12:04:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileIdent
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

CONFIG += c++11

LIBS += /usr/local/lib/libboost_system.so \
        /usr/local/lib/libboost_filesystem.so \
        /usr/local/lib/libboost_iostreams.so
