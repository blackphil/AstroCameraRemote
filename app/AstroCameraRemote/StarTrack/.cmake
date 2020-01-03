set(SOURCES_STARTRACK
	StarTrack/GraphicsScene.cpp
	StarTrack/GraphicsView.cpp
	StarTrack/LenseGraphcisScene.cpp
	StarTrack/Marker.cpp
	StarTrack/Settings.cpp
	StarTrack/StarInfo.cpp
	StarTrack/StarTrackView.cpp
	StarTrack/Tracker.cpp
)

set(HEADERS_STARTRACK
	StarTrack/GraphicsScene.h
	StarTrack/GraphicsView.h
	StarTrack/LenseGraphcisScene.h
	StarTrack/Marker.h
	StarTrack/Settings.h
	StarTrack/StarInfo.h
	StarTrack/StarTrackView.h
	StarTrack/Tracker.h
)
set(UIS_STARTRACK
	StarTrack/StarTrack_StarTrackView.ui
)
source_group("StarTrack" FILES ${SOURCES_STARTRACK} ${HEADERS_STARTRACK} ${UIS_STARTRACK})