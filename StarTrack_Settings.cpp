#include "StarTrack_Settings.h"
#include <QSettings>

namespace StarTrack {


float Settings::getFixedRectSize()
{
    return (float)QSettings().value("StarTrack/FixedRectSize", 100).toFloat();
}

void Settings::setFixedRectSize(float value)
{
    QSettings().setValue("StarTrack/FixedRectSize", value);
}

Marker::Modus Settings::getMarkerModus()
{
    return (Marker::Modus)QSettings().value("StarTrack/MarkerModus", Marker::Modus_Rubberband).toInt();
}

void Settings::setMarkerModus(const Marker::Modus &value)
{
    QSettings().setValue("StarTrack/MarkerModus", value);
}

} // namespace StarTrack
