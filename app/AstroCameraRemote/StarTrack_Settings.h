#ifndef STARTRACK_SETTINGS_H
#define STARTRACK_SETTINGS_H


#include <StarTrack_Marker.h>


namespace StarTrack {

class Settings
{


public:


    static Marker::Modus getMarkerModus();
    static void setMarkerModus(const Marker::Modus &value);
    static qreal getFixedRectSize();
    static void setFixedRectSize(qreal value);

    static void setPublishScaledImage(bool yes);
    static bool getPublishScaledImage();

    static int getHfdDisplayFontSize();
    static void setHfdDisplayFontSize(int size);
};

} // namespace StarTrack

#endif // STARTRACK_SETTINGS_H
