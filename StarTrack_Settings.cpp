#include "StarTrack_Settings.h"
#include <QSettings>

namespace StarTrack {


float Settings::getFixedRectSize()
{
    return QSettings().value("StarTrack/FixedRectSize", 100).toFloat();
}

void Settings::setFixedRectSize(float value)
{
    QSettings().setValue("StarTrack/FixedRectSize", value);
}

void Settings::setPublishScaledImage(bool yes)
{
    QSettings().setValue("StarTrack/PublishScaledImage", yes);
}

bool Settings::getPublishScaledImage()
{
    return QSettings().value("StarTrack/PublishScaledImage").toBool();
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
