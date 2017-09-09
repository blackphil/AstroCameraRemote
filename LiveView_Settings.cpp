#include "LiveView_Settings.h"

#include "SonyAlphaRemote_Helper.h"

namespace LiveView {

float Settings::getFps() const
{
    return fps;
}

void Settings::setFps(float value)
{
    fps = value;
}

void Settings::load()
{
    qSettings->beginGroup("LiveView");
    fps = qSettings->value("Fps", 10.0f).toFloat();
    qSettings->endGroup();

    Q_EMIT settingChanged();
}

void Settings::save()
{
    qSettings->beginGroup("LiveView");
    qSettings->setValue("Fps", fps);
    qSettings->endGroup();
}

QString Settings::getName()
{
    return "LiveView";
}

Settings::Settings(Setting *parent)
    : Setting(parent)
    , fps(10.0f)
{
    SAR_INF("ctor");
    setObjectName(getName());
}

} // namespace LiveView
