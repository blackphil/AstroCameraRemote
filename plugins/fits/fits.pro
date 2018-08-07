include($${top_srcdir}/common_pre.pri)


TEMPLATE=lib

TARGET=fits

QT += gui widgets

CONFIG += plugin

DEFINES += FITS_LIBRARY


DLL_DEPS += \
    AstroBase #\
    #BatchProcess


HEADERS += \
    fits.h \
    Fits_File.h \
    fits_global.h \
    Fits_ImageIOPlugin.h \
    Fits_ImageIOHandler.h

SOURCES += \
    Fits_File.cpp \
    Fits_ImageIOPlugin.cpp \
    Fits_ImageIOHandler.cpp


DISTFILES += \
    FitsMetadata.json

target.path += $$INSTALL_DEST_DIR/imageformats
INSTALL_FILES = target

include($${top_srcdir}/common_post.pri)
