TEMPLATE=lib

TARGET=fits

QT += gui widgets


DEFINES += FITS_LIBRARY


DLL_DEPS += \
    AstroBase \
    BatchProcess


HEADERS += \
    fits.h \
    Fits_Repair.h \
    Fits_File.h \
    fits_global.h

SOURCES += \
    Fits_Repair.cpp \
    Fits_File.cpp

include($${top_srcdir}/common_post.pri)

DISTFILES +=
