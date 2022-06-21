cmake_minimum_required(VERSION 3.20.0)

project(AstroBase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_definitions(-DASTROBASE_LIBRARY)
set(sources
    AstroBase.cpp
    AstroBase_Exception.cpp
    AstroBase_PersistentDirInfo.cpp
    AstroBase_MessageBox.cpp
)

set(headers
    AstroBase.h
    astrobase_global.h
    AstroBase_Exception.h
    AstroBase_PersistentDirInfo.h
    AstroBase_MessageBox.h
)

add_library(${PROJECT_NAME} SHARED ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
