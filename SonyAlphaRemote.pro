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
    SonyAlphaRemote_BulbShootSequencer.cpp \
    SonyAlphaRemote_Sequencer_StateBase.cpp \
    SonyAlphaRemote_Sequencer_StateWaitForStart.cpp \
    SonyAlphaRemote_Sequencer_StateNormalShooting.cpp \
    SonyAlphaRemote_Sequencer_StateBulbShooting.cpp \
    SonyAlphaRemote_Sequencer_StateWaitForCamReady.cpp \
    SonyAlphaRemote_Sequencer_StatePause.cpp \
    SonyAlphaRemote_Sequencer_StateFinish.cpp \
    SonyAlphaRemote_Json_GetEvent.cpp \
    SonyAlphaRemote_BatteryInfo.cpp \
    SonyAlphaRemote_Settings.cpp \
    SonyAlphaRemote_TimeUnitButton.cpp

HEADERS  += MainWindow.h \
    SonyAlphaRemote_Helper.h \
    SonyAlphaRemote_Json_Command.h \
    SonyAlphaRemote_Sender.h \
    SsdpClient.h \
    SonyAlphaRemote_StatusPoller.h \
    SonyAlphaRemote_BulbShootSequencer.h \
    SonyAlphaRemote_Sequencer_StateBase.h \
    SonyAlphaRemote_Sequencer_StateWaitForStart.h \
    SonyAlphaRemote_Sequencer_StateNormalShooting.h \
    SonyAlphaRemote_Sequencer_StateBulbShooting.h \
    SonyAlphaRemote_Sequencer_StateWaitForCamReady.h \
    SonyAlphaRemote_Sequencer_StatePause.h \
    SonyAlphaRemote_Sequencer_StateFinish.h \
    SonyAlphaRemote_Json_GetEvent.h \
    SonyAlphaRemote_BatteryInfo.h \
    SonyAlphaRemote_Settings.h \
    SonyAlphaRemote_TimeUnitButton.h

FORMS    += MainWindow.ui

DISTFILES += \
    ToDo.txt
