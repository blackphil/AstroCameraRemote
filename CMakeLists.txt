cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

project(AstroCameraRemoteProject)


include_directories("include")

add_subdirectory(app)
add_subdirectory(AstroBase)
add_subdirectory(BatchProcess)
add_subdirectory(DataFormat)
add_subdirectory(EasyExif)

add_dependencies(BatchProcess AstroBase EasyExif Fits)
add_dependencies(AstroCameraRemote AstroBase BatchProcess EasyExif Fits)
add_dependencies(Fits AstroBase)

install(TARGETS AstroCameraRemote
    AstroBase
    BatchProcess
    EasyExif
    Fits
    )

