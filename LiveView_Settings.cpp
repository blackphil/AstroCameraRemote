#include "LiveView_Settings.h"

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

Settings::Settings(Setting *parent)
    : Setting(parent)
    , fps(10.0f)
{

}

} // namespace LiveView
