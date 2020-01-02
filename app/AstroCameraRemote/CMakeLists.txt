cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)


add_executable(AstroCameraRemote
	resources.qrc
	testdata_1.qrc
	testdata_2.qrc
	testdata_3.qrc
	testdata_4.qrc
	testdata_5.qrc
	hfd/Hfd_Calculator.cpp
	hfd/Hfd_Calculator.h
	BatchProcess_FitsRepair.cpp
	BatchProcess_FitsRepair.h
	BatteryInfo.cpp
	BatteryInfo.h
	Helper.cpp
	Helper.h
	ImageView.cpp
	ImageView.h
	ImageViewZoomControl.cpp
	ImageViewZoomControl.h
	Json_Command.cpp
	Json_Command.h
	Json_GetEvent.cpp
	Json_GetEvent.h
	LiveView_Commands.cpp
	LiveView_Commands.h
	LiveView_ImageQueue.cpp
	LiveView_ImageQueue.h
	LiveView_Info.cpp
	LiveView_Info.h
	LiveView_Reader.cpp
	LiveView_Reader.h
	LiveView_ReaderThread.cpp
	LiveView_ReaderThread.h
	LiveView_Settings.cpp
	LiveView_Settings.h
	LiveView_Widget.cpp
	LiveView_Widget.h
	main.cpp
	MainWindow.cpp
	MainWindow.h
	MessagePoster.cpp
	MessagePoster.h
	PostView_ImageLoader.cpp
	PostView_ImageLoader.h
	PostView_Info.cpp
	PostView_Info.h
	PostView_Widget.cpp
	PostView_Widget.h
	Sender.cpp
	Sender.h
	Sequencer_Base.cpp
	Sequencer_Base.h
	Sequencer_BulbShootSequencer.cpp
	Sequencer_BulbShootSequencer.h
	Sequencer_ControlWidget.cpp
	Sequencer_ControlWidget.h
	Sequencer_CreateNewDialog.cpp
	Sequencer_CreateNewDialog.h
	Sequencer_NormalShootSequencer.cpp
	Sequencer_NormalShootSequencer.h
	Sequencer_PhotoShot.cpp
	Sequencer_PhotoShot.h
	Sequencer_Properties.cpp
	Sequencer_Properties.h
	Sequencer_Protocol.cpp
	Sequencer_Protocol.h
	Sequencer_ProtocolModel.cpp
	Sequencer_ProtocolModel.h
	Sequencer_ProtocolView.cpp
	Sequencer_ProtocolView.h
	Sequencer_StateBase.cpp
	Sequencer_StateBase.h
	Sequencer_StateBulbShooting.cpp
	Sequencer_StateBulbShooting.h
	Sequencer_StateFinish.cpp
	Sequencer_StateFinish.h
	Sequencer_StateNormalShooting.cpp
	Sequencer_StateNormalShooting.h
	Sequencer_StatePause.cpp
	Sequencer_StatePause.h
	Sequencer_StateWaitForCamReady.cpp
	Sequencer_StateWaitForCamReady.h
	Sequencer_StateWaitForStart.cpp
	Sequencer_StateWaitForStart.h
	Settings.cpp
	Settings.h
	Settings_General.cpp
	Settings_General.h
	SettingsDialog.cpp
	SettingsDialog.h
	SsdpClient.cpp
	SsdpClient.h
	StarTrack_GraphicsScene.cpp
	StarTrack_GraphicsScene.h
	StarTrack_GraphicsView.cpp
	StarTrack_GraphicsView.h
	StarTrack_LenseGraphcisScene.cpp
	StarTrack_LenseGraphcisScene.h
	StarTrack_Marker.cpp
	StarTrack_Marker.h
	StarTrack_Settings.cpp
	StarTrack_Settings.h
	StarTrack_StarInfo.cpp
	StarTrack_StarInfo.h
	StarTrack_StarTrackView.cpp
	StarTrack_StarTrackView.h
	StarTrack_Tracker.cpp
	StarTrack_Tracker.h
	StatusPoller.cpp
	StatusPoller.h
	TimeUnit.cpp
	TimeUnit.h
	TimeUnitButton.cpp
	TimeUnitButton.h
	ToolBar.cpp
	ToolBar.h
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

