#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T19:54:30
#
#-------------------------------------------------
include($${top_srcdir}/common_pre.pri)


QT       += core gui network widgets

TARGET = AstroCameraRemote
TEMPLATE = app

QTPLUGIN += svg


DLL_DEPS += \
    AstroBase \
    BatchProcess \
    DataFormat/Fits

SOURCES += main.cpp\
        MainWindow.cpp \
    SsdpClient.cpp \
    SettingsDialog.cpp \
    Settings_General.cpp \
    ToolBar.cpp \
    PostView_Widget.cpp \
    PostView_Info.cpp \
    LiveView_Widget.cpp \
    LiveView_Reader.cpp \
    LiveView_Commands.cpp \
    LiveView_ReaderThread.cpp \
    LiveView_Info.cpp \
    hfd/Hfd_Calculator.cpp \
    LiveView_ImageQueue.cpp \
    LiveView_Settings.cpp \
    StarTrack_Marker.cpp \
    StarTrack_GraphicsScene.cpp \
    StarTrack_Settings.cpp \
    StarTrack_LenseGraphcisScene.cpp \
    BatchProcess_FitsRepair.cpp \
    PostView_ImageLoader.cpp \
    StarTrack_StarTrackView.cpp \
    StarTrack_GraphicsView.cpp \
    StarTrack_StarInfo.cpp \
    StarTrack_Tracker.cpp \
    ImageView.cpp \
    Sequencer_ControlWidget.cpp \
    MessagePoster.cpp \
    BatteryInfo.cpp \
    Helper.cpp \
    Json_Command.cpp \
    Json_GetEvent.cpp \
    Sender.cpp \
    Sequencer_Base.cpp \
    Sequencer_BulbShootSequencer.cpp \
    Sequencer_NormalShootSequencer.cpp \
    Sequencer_Settings.cpp \
    Sequencer_SettingsManager.cpp \
    Sequencer_StateBase.cpp \
    Sequencer_StateBulbShooting.cpp \
    Sequencer_StateFinish.cpp \
    Sequencer_StateNormalShooting.cpp \
    Sequencer_StatePause.cpp \
    Sequencer_StateWaitForCamReady.cpp \
    Sequencer_StateWaitForStart.cpp \
    Settings.cpp \
    StatusPoller.cpp \
    TimeUnitButton.cpp \
    Sequencer_Protocol.cpp

HEADERS  += MainWindow.h \
    SsdpClient.h \
    SettingsDialog.h \
    Settings_General.h \
    ToolBar.h \
    PostView_Widget.h \
    PostView_Info.h \
    LiveView_Widget.h \
    LiveView_Reader.h \
    LiveView_Commands.h \
    LiveView_ReaderThread.h \
    LiveView_Info.h \
    hfd/Hfd_Calculator.h \
    LiveView_ImageQueue.h \
    LiveView_Settings.h \
    StarTrack_Marker.h \
    StarTrack_GraphicsScene.h \
    StarTrack_Settings.h \
    StarTrack_LenseGraphcisScene.h \
    BatchProcess_FitsRepair.h \
    PostView_ImageLoader.h \
    StarTrack_StarTrackView.h \
    StarTrack_GraphicsView.h \
    StarTrack_StarInfo.h \
    StarTrack_Tracker.h \
    ImageView.h \
    Sequencer_ControlWidget.h \
    MessagePoster.h \
    BatteryInfo.h \
    Helper.h \
    Json_Command.h \
    Json_GetEvent.h \
    Sender.h \
    Sequencer_Base.h \
    Sequencer_BulbShootSequencer.h \
    Sequencer_NormalShootSequencer.h \
    Sequencer_Settings.h \
    Sequencer_SettingsManager.h \
    Sequencer_StateBase.h \
    Sequencer_StateBulbShooting.h \
    Sequencer_StateFinish.h \
    Sequencer_StateNormalShooting.h \
    Sequencer_StatePause.h \
    Sequencer_StateWaitForCamReady.h \
    Sequencer_StateWaitForStart.h \
    Settings.h \
    StatusPoller.h \
    TimeUnitButton.h \
    Sequencer_Protocol.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui \
    PostView_Widget.ui \
    LiveView_Widget.ui \
    StarTrack_StarTrackView.ui \
    Sequencer_ControlWidget.ui

#DEFINES += DRY_RUN

DISTFILES += \
    ToDo.txt \
    features/win32/setup.prf

RESOURCES += \
    resources.qrc \
    testdata_1.qrc \
    testdata_2.qrc \
    testdata_3.qrc \
    testdata_4.qrc \
    testdata_5.qrc

INSTALL_FILES = target

include($${top_srcdir}/common_post.pri)

