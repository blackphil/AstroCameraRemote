#ifndef STARTRACK_SETTINGS_H
#define STARTRACK_SETTINGS_H

#include <SonyAlphaRemote_Settings.h>
#include <StarTrack_Marker.h>


namespace StarTrack {

class Settings : public SonyAlphaRemote::Setting
{
    Q_OBJECT

    Marker::Modus markerModus;
    int fixedRectSize;

public:
    Settings(Setting* parent);

    static QString getName();

    void load();
    void save();

    Marker::Modus getMarkerModus() const;
    void setMarkerModus(const Marker::Modus &value);
    int getFixedRectSize() const;
    void setFixedRectSize(int value);
};

} // namespace StarTrack

#endif // STARTRACK_SETTINGS_H
