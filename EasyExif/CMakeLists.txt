cmake_minimum_required(VERSION 3.20.0)

project(EasyExif VERSION 1.0.0 LANGUAGES CXX)

include(Qt)
include(msvc)

add_definitions(-DEASYEXIF_LIBRARY)

set(sources
    EasyExif_Exif.cpp

)

set(headers
	EasyExif_Exif.h
	easyexif_global.h 
)

add_library(${PROJECT_NAME} SHARED ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
		${QT6_LIBRARIES}
)
