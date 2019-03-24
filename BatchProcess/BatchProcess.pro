#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T12:24:37
#
#-------------------------------------------------

include($${top_srcdir}/common_pre.pri)


QT       += widgets

TARGET = BatchProcess
TEMPLATE = lib

DLL_DEPS += AstroBase DataFormat/Fits EasyExif

DEFINES += BATCHPROCESS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        BatchProcess.cpp \
    BatchProcess_SelectFilesDialog.cpp \
    BatchProcess_Processor.cpp \
    BatchProcess_Visual_GraphicsScene.cpp \
    BatchProcess_ImageSignal.cpp \
    BatchProcess_Task.cpp \
    BatchProcess_StackImagesTask.cpp \
    BatchProcess_PixelMathTask.cpp \
    BatchProcess_Visual_TaskBox.cpp \
    BatchProcess_Signal.cpp \
    BatchProcess_NumericSignal.cpp \
    BatchProcess_Manager.cpp \
    BatchProcess_NumericValueDialog.cpp \
    BatchProcess_Visual_Connection.cpp \
    BatchProcess_Visual_Pin.cpp \
    BatchProcess_SaveImagesTask.cpp \
    BatchProcess_SaveImagesEditDlg.cpp \
    BatchProcess_RawImageGrabber.cpp

HEADERS += \
        BatchProcess.h \
        batchprocess_global.h \
    BatchProcess_SelectFilesDialog.h \
    BatchProcess_Processor.h \
    BatchProcess_TaskInterface.h \
    BatchProcess_Visual_GraphicsScene.h \
    BatchProcess_ImageSignal.h \
    BatchProcess_Task.h \
    BatchProcess_StackImagesTask.h \
    BatchProcess_PixelMathTask.h \
    BatchProcess_Visual_TaskBox.h \
    BatchProcess_Signal.h \
    BatchProcess_NumericSignal.h \
    BatchProcess_Manager.h \
    BatchProcess_NumericValueDialog.h \
    BatchProcess_Visual_Connection.h \
    BatchProcess_Visual_Pin.h \
    BatchProcess_SaveImagesTask.h \
    BatchProcess_SaveImagesEditDlg.h \
    BatchProcess_RawImageGrabber.h

FORMS += \
    BatchProcess_SelectFilesDialog.ui \
    BatchProcess_NumericValueDialog.ui \
    BatchProcess_SaveImagesEditDlg.ui

INSTALL_FILES = target

include($${top_srcdir}/common_post.pri)

