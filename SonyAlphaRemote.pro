#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T19:54:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SonyAlphaRemote
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SonyAlphaRemote_Helper.cpp \
    SonyAlphaRemote_Json_Command.cpp \
    SonyAlphaRemote_Sender.cpp \
    SsdpClient.cpp \
    SonyAlphaRemote_StatusPoller.cpp \
    SonyAlphaRemote_BulbShootSequencer.cpp

HEADERS  += MainWindow.h \
    SonyAlphaRemote_Helper.h \
    SonyAlphaRemote_Json_Command.h \
    SonyAlphaRemote_Sender.h \
    SsdpClient.h \
    SonyAlphaRemote_StatusPoller.h \
    SonyAlphaRemote_BulbShootSequencer.h

FORMS    += MainWindow.ui
