cmake_minimum_required(VERSION 3.16)

project(AstroCameraRemote)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(app)
add_subdirectory(AstroBase)
add_subdirectory(BatchProcess)
add_subdirectory(DataFormat)
add_subdirectory(EasyExif)
#add_subdirectory(plugins)

install(TARGETS AstroCameraRemote
        CONFIGURATIONS Debug
        RUNTIME DESTINATION D:/philipp/dev/install/Debug/bin)
install(TARGETS AstroCameraRemote
        CONFIGURATIONS Release
        RUNTIME DESTINATION D:/philipp/dev/install/Release/bin)
	
install(SCRIPT dependencies.cmake)