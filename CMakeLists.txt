cmake_minimum_required(VERSION 3.20.0)

project(AstroCameraRemoteProject)

find_package(Qt6 REQUIRED COMPONENTS Core)
#qt_standard_project_setup()

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

