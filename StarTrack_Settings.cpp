#include "StarTrack_Settings.h"
#include "SonyAlphaRemote_Helper.h"

namespace StarTrack {

void Settings::load()
{
    SAR_INF("BEGIN");
    qSettings->beginGroup("StarTrack");
    markerModus = (Marker::Modus)qSettings->value("MarkerModus", Marker::Modus_Rubberband).toInt();
    fixedRectSize = (float)qSettings->value("FixedRectSize", 100).toInt();
    qSettings->endGroup();

    Q_EMIT settingChanged();
}

void Settings::save()
{
    SAR_INF("BEGIN");
    qSettings->beginGroup("StarTrack");
    qSettings->setValue("MarkerModus", markerModus);
    qSettings->setValue("FixedRectSize", fixedRectSize);
    qSettings->endGroup();
}

Settings::Settings(Setting *parent)
    : Setting(parent)
    , markerModus(Marker::Modus_Rubberband)
    , fixedRectSize(100)
{
    SAR_INF("ctor");
    setObjectName(getName());
}

QString Settings::getName()
{
    return "StarTrack";
}

int Settings::getFixedRectSize() const
{
    return fixedRectSize;
}

void Settings::setFixedRectSize(int value)
{
    fixedRectSize = value;
}

Marker::Modus Settings::getMarkerModus() const
{
    return markerModus;
}

void Settings::setMarkerModus(const Marker::Modus &value)
{
    markerModus = value;
}

} // namespace StarTrack
