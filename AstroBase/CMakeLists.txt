cmake_minimum_required(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(AstroBase STATIC
	AstroBase.cpp
	AstroBase.h
	AstroBase_Exception.cpp
	AstroBase_Exception.h
	AstroBase_MessageBox.cpp
	AstroBase_MessageBox.h
	AstroBase_PersistentDirInfo.cpp
	AstroBase_PersistentDirInfo.h
)
target_include_directories (AstroBase PUBLIC .)
target_link_libraries(AstroBase Qt5::Widgets)