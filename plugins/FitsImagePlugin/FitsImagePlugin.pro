include($${top_srcdir}/common_pre.pri)


TEMPLATE=lib

TARGET=FitsImagePlugin

QT += gui widgets

CONFIG += plugin

DEFINES += FITS_LIBRARY


DLL_DEPS += \
    AstroBase \
    DataFormat/Fits


HEADERS += \
    fitsimageplugin.h \
    fitsimageplugin_global.h \
    FitsImagePlugin_ImageIOPlugin.h \
    FitsImagePlugin_ImageIOHandler.h

SOURCES += \
    FitsImagePlugin_ImageIOPlugin.cpp \
    FitsImagePlugin_ImageIOHandler.cpp


DISTFILES += \
    FitsImagePluginMetadata.json

target.path += $$INSTALL_DEST_DIR/imageformats
INSTALL_FILES = target

include($${top_srcdir}/common_post.pri)
