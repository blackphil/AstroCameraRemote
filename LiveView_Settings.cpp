#include "LiveView_Settings.h"

#include <QSettings>

#include "SonyAlphaRemote_Helper.h"

namespace LiveView {

float Settings::getFps()
{
    return QSettings().value("LiveView/Fps", 10.0f).toFloat();
}

void Settings::setFps(float value)
{
    QSettings().setValue("LiveView/Fps", value);
}

} // namespace LiveView
