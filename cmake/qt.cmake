# minimum version required for proper support of C++11 features in Qt
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND QT6_MODULES
    Core
    Gui
    Network
    StateMachine
    Svg
    Widgets
    Xml
)

source_group("Resources" REGULAR_EXPRESSION ".*\.qrc")
source_group("Forms"     REGULAR_EXPRESSION ".*\.ui")


find_package(Qt6 REQUIRED COMPONENTS ${QT6_MODULES})

foreach(module IN ITEMS ${QT6_MODULES})
    list(APPEND QT6_LIBRARIES Qt6::${module})
endforeach()

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

function(custom_qt_deploy target)

    if(MSVC)
        get_target_property(_qmake_exe Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_exe}" DIRECTORY)

        #get the platform plugin location
        set(_qt_platform_dir "${_qt_bin_dir}/../plugins/platforms")
        set(_qt_win32_platform_plugin "${_qt_platform_dir}/qwindows$<$<CONFIG:Debug>:d>.dll")
        message(STATUS "_qt_win32_platform_plugin: ${_qt_win32_platform_plugin}")

        foreach(target_file IN ITEMS ${QT6_LIBRARIES})
            list(APPEND target_files "$<TARGET_FILE:${target_file}>")
        endforeach()

        get_target_property(_target_bin_dir ${PROJECT_NAME} BINARY_DIR)

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${target_files}
            "${_target_bin_dir}/$<CONFIG>"
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${_target_bin_dir}/$<CONFIG>/platforms"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${_qt_win32_platform_plugin}
            "${_target_bin_dir}/$<CONFIG>/platforms"
            COMMENT "Deploying Qt libraries for target '${target}' ..."
        )
endif()

endfunction()
