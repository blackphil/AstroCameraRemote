cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(Fits STATIC
	Fits_File.cpp
	Fits_File.h
)

target_include_directories (Fits PUBLIC .)

add_dependencies(Fits AstroBase)

target_link_libraries(Fits AstroBase)	
target_link_libraries(Fits Qt5::Widgets)
