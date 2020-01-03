set(SOURCES_LIVEVIEW
	LiveView/Commands.cpp
	LiveView/ImageQueue.cpp
	LiveView/Info.cpp
	LiveView/Reader.cpp
	LiveView/ReaderThread.cpp
	LiveView/Settings.cpp
	LiveView/Widget.cpp
)

set(HEADERS_LIVEVIEW
	LiveView/Commands.h
	LiveView/ImageQueue.h
	LiveView/Info.h
	LiveView/Reader.h
	LiveView/ReaderThread.h
	LiveView/Settings.h
	LiveView/Widget.h
)
set(UIS_LIVEVIEW
	LiveView/LiveView_Widget.ui
)
	
source_group("LiveView" FILES ${SOURCES_LIVEVIEW} ${HEADERS_LIVEVIEW} ${UIS_LIVEVIEW})
