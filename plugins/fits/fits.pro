TEMPLATE=lib

TARGET=fits

QT += gui widgets

CONFIG += plugin

DEFINES += FITS_LIBRARY


DLL_DEPS += \
    AstroBase \
    BatchProcess


HEADERS += \
    fits.h \
    Fits_Repair.h \
    Fits_File.h \
    fits_global.h \
    Fits_ImageIOPlugin.h \
    Fits_ImageIOHandler.h

SOURCES += \
    Fits_Repair.cpp \
    Fits_File.cpp \
    Fits_ImageIOPlugin.cpp \
    Fits_ImageIOHandler.cpp

include($${top_srcdir}/common_post.pri)

DISTFILES += \
    FitsMetadata.json
