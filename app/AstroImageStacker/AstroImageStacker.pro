#-------------------------------------------------
#
# Project created by QtCreator 2018-08-11T20:41:22
#
#-------------------------------------------------
include($${top_srcdir}/common_pre.pri)

QT       += core gui widgets

TARGET = AstroImageStacker
TEMPLATE = app

CONFIG += c++11

DLL_DEPS += \
    AstroBase \
    Plugins/Fits \
    BatchProcess

SOURCES += \
        main.cpp \
        MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
        MainWindow.ui


INSTALL_FILES = target

include($${top_srcdir}/common_post.pri)

