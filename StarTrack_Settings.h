#ifndef STARTRACK_SETTINGS_H
#define STARTRACK_SETTINGS_H


#include <StarTrack_Marker.h>


namespace StarTrack {

class Settings
{


public:


    static Marker::Modus getMarkerModus();
    static void setMarkerModus(const Marker::Modus &value);
    static float getFixedRectSize();
    static void setFixedRectSize(float value);
};

} // namespace StarTrack

#endif // STARTRACK_SETTINGS_H
