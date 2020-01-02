cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)

add_library(BatchProcess STATIC
	BatchProcess.cpp
	BatchProcess.h
	BatchProcess_ImageSignal.cpp
	BatchProcess_ImageSignal.h
	BatchProcess_Manager.cpp
	BatchProcess_Manager.h
	BatchProcess_NumericSignal.cpp
	BatchProcess_NumericSignal.h
	BatchProcess_NumericValueDialog.cpp
	BatchProcess_NumericValueDialog.h
	BatchProcess_NumericValueDialog.ui
	BatchProcess_PixelMathTask.cpp
	BatchProcess_PixelMathTask.h
	BatchProcess_Processor.cpp
	BatchProcess_Processor.h
	BatchProcess_RawImageGrabber.cpp
	BatchProcess_RawImageGrabber.h
	BatchProcess_SaveImagesEditDlg.cpp
	BatchProcess_SaveImagesEditDlg.h
	BatchProcess_SaveImagesEditDlg.ui
	BatchProcess_SaveImagesTask.cpp
	BatchProcess_SaveImagesTask.h
	BatchProcess_SelectFilesDialog.cpp
	BatchProcess_SelectFilesDialog.h
	BatchProcess_SelectFilesDialog.ui
	BatchProcess_Signal.cpp
	BatchProcess_Signal.h
	BatchProcess_StackImagesTask.cpp
	BatchProcess_StackImagesTask.h
	BatchProcess_Task.cpp
	BatchProcess_Task.h
	BatchProcess_TaskInterface.h
	BatchProcess_Visual_Connection.cpp
	BatchProcess_Visual_Connection.h
	BatchProcess_Visual_GraphicsScene.cpp
	BatchProcess_Visual_GraphicsScene.h
	BatchProcess_Visual_Pin.cpp
	BatchProcess_Visual_Pin.h
	BatchProcess_Visual_TaskBox.cpp
	BatchProcess_Visual_TaskBox.h
)
target_include_directories (BatchProcess PUBLIC .)

add_dependencies(BatchProcess AstroBase Fits EasyExif)

target_link_libraries(BatchProcess AstroBase Fits EasyExif)
target_link_libraries(BatchProcess Qt5::Gui Qt5::Widgets)