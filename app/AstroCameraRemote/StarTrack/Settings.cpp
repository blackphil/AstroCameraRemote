#include "Settings.h"
#include <QSettings>

namespace StarTrack {


qreal Settings::getFixedRectSize()
{
    return QSettings().value("StarTrack/FixedRectSize", 100).toReal();
}

void Settings::setFixedRectSize(qreal value)
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
    return static_cast<Marker::Modus>(QSettings().value("StarTrack/MarkerModus", Marker::Modus_Rubberband).toInt());
}

void Settings::setMarkerModus(const Marker::Modus &value)
{
    QSettings().setValue("StarTrack/MarkerModus", value);
}

int Settings::getHfdDisplayFontSize()
{
    return QSettings().value("StarTrack/HfdDisplayFontSize", 20).toInt();
}

void Settings::setHfdDisplayFontSize(int size)
{
    QSettings().setValue("StarTrack/HfdDisplayFontSize", size);
}

} // namespace StarTrack
