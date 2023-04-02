cmake_minimum_required(VERSION 3.20.0)

project(BatchProcess VERSION 1.0.0 LANGUAGES CXX)

include(Qt)
include(msvc)

add_definitions(-DBATCHPROCESS_LIBRARY)

set(sources
    BatchProcess.cpp 
    BatchProcess_SelectFilesDialog.cpp 
    BatchProcess_Processor.cpp 
    BatchProcess_Visual_GraphicsScene.cpp 
    BatchProcess_ImageSignal.cpp 
    BatchProcess_Task.cpp 
    BatchProcess_StackImagesTask.cpp 
    BatchProcess_PixelMathTask.cpp 
    BatchProcess_Visual_TaskBox.cpp 
    BatchProcess_Signal.cpp 
    BatchProcess_NumericSignal.cpp 
    BatchProcess_Manager.cpp 
    BatchProcess_NumericValueDialog.cpp 
    BatchProcess_Visual_Connection.cpp 
    BatchProcess_Visual_Pin.cpp 
    BatchProcess_SaveImagesTask.cpp 
    BatchProcess_SaveImagesEditDlg.cpp 
    BatchProcess_RawImageGrabber.cpp
)

set(headers
	BatchProcess.h 
	batchprocess_global.h 
    BatchProcess_SelectFilesDialog.h 
    BatchProcess_Processor.h 
    BatchProcess_TaskInterface.h 
    BatchProcess_Visual_GraphicsScene.h 
    BatchProcess_ImageSignal.h 
    BatchProcess_Task.h 
    BatchProcess_StackImagesTask.h 
    BatchProcess_PixelMathTask.h 
    BatchProcess_Visual_TaskBox.h 
    BatchProcess_Signal.h 
    BatchProcess_NumericSignal.h 
    BatchProcess_Manager.h 
    BatchProcess_NumericValueDialog.h 
    BatchProcess_Visual_Connection.h 
    BatchProcess_Visual_Pin.h 
    BatchProcess_SaveImagesTask.h 
    BatchProcess_SaveImagesEditDlg.h 
    BatchProcess_RawImageGrabber.h
)

set(forms
    BatchProcess_SelectFilesDialog.ui 
    BatchProcess_NumericValueDialog.ui 
    BatchProcess_SaveImagesEditDlg.ui
)

add_library(${PROJECT_NAME} SHARED ${sources} ${headers} ${forms})
target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
		${QT6_LIBRARIES}
		AstroBase 
		EasyExif 
		Fits
)
