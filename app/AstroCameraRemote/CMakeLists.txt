cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)


set(HEADERS_
	BatchProcess_FitsRepair.h
	BatteryInfo.h
	Helper.h
	ImageView.h
	ImageViewZoomControl.h
	MainWindow.h
	MessagePoster.h
	Sender.h
	Settings.h
	Settings_General.h
	SettingsDialog.h
	SsdpClient.h
	StatusPoller.h
	TimeUnit.h
	TimeUnitButton.h
	ToolBar.h
)
set(SOURCES_
	BatchProcess_FitsRepair.cpp
	BatteryInfo.cpp
	Helper.cpp
	ImageView.cpp
	ImageViewZoomControl.cpp
	main.cpp
	MainWindow.cpp
	MessagePoster.cpp
	Sender.cpp
	Settings.cpp
	Settings_General.cpp
	SettingsDialog.cpp
	SsdpClient.cpp
	StatusPoller.cpp
	TimeUnit.cpp
	TimeUnitButton.cpp
	ToolBar.cpp
)

set(RESOURCES_
	resources.qrc
	testdata_1.qrc
	testdata_2.qrc
	testdata_3.qrc
	testdata_4.qrc
	testdata_5.qrc
)

set(UIS_
	ImageViewZoomControl.ui
	MainWindow.ui
	SettingsDialog.ui
)	

include(hfd/.cmake)
include(Json/.cmake)
include(Sequencer/.cmake)
include(StarTrack/.cmake)
include(LiveView/.cmake)
include(PostView/.cmake)


add_executable(AstroCameraRemote

	${HEADERS_}
	${RESOURCES_}
	${SOURCES_}
	${UIS_}
	
	${HEADERS_HFD}
	${SOURCES_HFD}
	${UIS_HFD}
	
	${HEADERS_JSON}
	${SOURCES_JSON}
	${UIS_JSON}
	
	${HEADERS_LIVEVIEW}
	${SOURCES_LIVEVIEW}
	${UIS_LIVEVIEW}
	
	${HEADERS_POSTVIEW}
	${SOURCES_POSTVIEW}
	${UIS_POSTVIEW}
	
	${HEADERS_SEQUENCER}
	${SOURCES_SEQUENCER}
	${UIS_SEQUENCER}
	
	${HEADERS_STARTRACK}
	${SOURCES_STARTRACK}
	${UIS_STARTRACK}
	)
	
	
add_dependencies(AstroCameraRemote AstroBase BatchProcess Fits)
	
target_link_libraries(AstroCameraRemote AstroBase BatchProcess Fits)
target_link_libraries(AstroCameraRemote Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Xml)


if (WIN32)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    add_custom_command(TARGET AstroCameraRemote POST_BUILD
       COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:AstroCameraRemote>)
endif(WIN32)

