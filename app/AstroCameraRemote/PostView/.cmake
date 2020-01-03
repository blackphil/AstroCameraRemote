set(SOURCES_POSTVIEW
	PostView/ImageLoader.cpp
	PostView/Info.cpp
	PostView/Widget.cpp
)

set(HEADERS_POSTVIEW
	PostView/ImageLoader.h
	PostView/Info.h
	PostView/Widget.h
)

set(UIS_POSTVIEW
	PostView/PostView_Widget.ui
)

source_group("PostView" FILES ${SOURCES_POSTVIEW} ${HEADERS_POSTVIEW} ${UIS_POSTVIEW})