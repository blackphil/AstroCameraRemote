cmake_minimum_required(VERSION 3.20.0)

project(AstroCameraRemote VERSION 1.0.0 LANGUAGES CXX)

include(Qt)
include(msvc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(sources 
    main.cpp
    MainWindow.cpp
    SsdpClient.cpp
    SettingsDialog.cpp
    Settings_General.cpp
    ToolBar.cpp
    PostView_Widget.cpp
    PostView_Info.cpp
    LiveView_Widget.cpp
    LiveView_Reader.cpp
    LiveView_Commands.cpp
    LiveView_ReaderThread.cpp
    LiveView_Info.cpp
    hfd/Hfd_Calculator.cpp
    LiveView_ImageQueue.cpp
    LiveView_Settings.cpp
    StarTrack_Marker.cpp
    StarTrack_GraphicsScene.cpp
    StarTrack_Settings.cpp
    StarTrack_LenseGraphcisScene.cpp
    BatchProcess_FitsRepair.cpp
    PostView_ImageLoader.cpp
    StarTrack_StarTrackView.cpp
    StarTrack_GraphicsView.cpp
    StarTrack_StarInfo.cpp
    StarTrack_Tracker.cpp
    ImageView.cpp
    Sequencer_ControlWidget.cpp
    MessagePoster.cpp
    BatteryInfo.cpp
    Helper.cpp
    Json_Command.cpp
    Json_GetEvent.cpp
    Sender.cpp
    Sequencer_Base.cpp
    Sequencer_BulbShootSequencer.cpp
    Sequencer_NormalShootSequencer.cpp
    Sequencer_StateBase.cpp
    Sequencer_StateBulbShooting.cpp
    Sequencer_StateFinish.cpp
    Sequencer_StateNormalShooting.cpp
    Sequencer_StatePause.cpp
    Sequencer_StateWaitForCamReady.cpp
    Sequencer_StateWaitForStart.cpp
    Settings.cpp
    StatusPoller.cpp
    TimeUnitButton.cpp
    Sequencer_Protocol.cpp
    Sequencer_ProtocolModel.cpp
    Sequencer_Properties.cpp
    Sequencer_ProtocolView.cpp
    ImageViewZoomControl.cpp
    Sequencer_CreateNewDialog.cpp
    Sequencer_PhotoShot.cpp
    TimeUnit.cpp
    )

set(headers
    MainWindow.h
    SsdpClient.h
    SettingsDialog.h
    Settings_General.h
    ToolBar.h
    PostView_Widget.h
    PostView_Info.h
    LiveView_Widget.h
    LiveView_Reader.h
    LiveView_Commands.h
    LiveView_ReaderThread.h
    LiveView_Info.h
    hfd/Hfd_Calculator.h
    LiveView_ImageQueue.h
    LiveView_Settings.h
    StarTrack_Marker.h
    StarTrack_GraphicsScene.h
    StarTrack_Settings.h
    StarTrack_LenseGraphcisScene.h
    BatchProcess_FitsRepair.h
    PostView_ImageLoader.h
    StarTrack_StarTrackView.h
    StarTrack_GraphicsView.h
    StarTrack_StarInfo.h
    StarTrack_Tracker.h
    ImageView.h
    Sequencer_ControlWidget.h
    MessagePoster.h
    BatteryInfo.h
    Helper.h
    Json_Command.h
    Json_GetEvent.h
    Sender.h
    Sequencer_Base.h
    Sequencer_BulbShootSequencer.h
    Sequencer_NormalShootSequencer.h
    Sequencer_StateBase.h
    Sequencer_StateBulbShooting.h
    Sequencer_StateFinish.h
    Sequencer_StateNormalShooting.h
    Sequencer_StatePause.h
    Sequencer_StateWaitForCamReady.h
    Sequencer_StateWaitForStart.h
    Settings.h
    StatusPoller.h
    TimeUnitButton.h
    Sequencer_Protocol.h
    Sequencer_ProtocolModel.h
    Sequencer_Properties.h
    Sequencer_ProtocolView.h
    ImageViewZoomControl.h
    Sequencer_CreateNewDialog.h
    Sequencer_PhotoShot.h
    TimeUnit.h
    )

set(forms
    MainWindow.ui
    SettingsDialog.ui
    PostView_Widget.ui
    LiveView_Widget.ui
    StarTrack_StarTrackView.ui
    Sequencer_ControlWidget.ui
    ImageViewZoomControl.ui
    Sequencer_CreateNewDialog.ui
    )
source_group("Forms" ${forms})

set(resources
    resources.qrc
    testdata_1.qrc
    testdata_2.qrc
    testdata_3.qrc
    testdata_4.qrc
    testdata_5.qrc
)
source_group("Resources" ${resources})

qt_add_executable(${PROJECT_NAME} ${sources} ${headers} ${forms} ${resources})

list(APPEND OWN_LIBRARIES
    AstroBase
    BatchProcess
    EasyExif
    Fits
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${QT6_LIBRARIES}
    ${OWN_LIBRARIES}
    )

set(WINDEPLOYQT_DIR "${CMAKE_CURRENT_BINARY_DIR}/windeployqt")
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E remove_directory ${WINDEPLOYQT_DIR}
    COMMAND ${WINDEPLOYQT_EXECUTABLE} --no-translations --dir ${WINDEPLOYQT_DIR} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
)

install(
    DIRECTORY
    "${WINDEPLOYQT_DIR}/"
    DESTINATION bin
)

#custom_qt_deploy(${PROJECT_NAME})

#get_target_property(_target_bin_dir ${PROJECT_NAME} BINARY_DIR)

#foreach(own_lib_file IN ITEMS ${OWN_LIBRARIES})
#    list(APPEND own_lib_files "$<TARGET_FILE:${own_lib_file}>")
#endforeach()

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_if_different
#		${own_lib_files}
#		"${_target_bin_dir}/$<CONFIG>"
#	COMMENT "Deploying own libraries for target '${target}' ..."
#)
