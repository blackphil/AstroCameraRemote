cmake_minimum_required(VERSION 3.20.0)

project(Fits VERSION 1.0.0 LANGUAGES CXX)

include(Qt)
include(msvc)

add_definitions(-DFITS_LIBRARY)

set(sources
	fits.cpp 
	Fits_File.cpp
)

set(headers
	fits.h
	fits_global.h
	Fits_File.h
)

add_library(${PROJECT_NAME} SHARED ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
		${QT6_LIBRARIES}
		AstroBase
)
