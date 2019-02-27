#include "LiveView_Settings.h"

#include <QSettings>

#include "SonyAlphaRemote_Helper.h"

namespace LiveView {

int Settings::getFps()
{
    bool ok = false;
    QVariant value = QSettings().value("LiveView/Fps", 10);
    int result = value.toInt(&ok);
    if(!ok)
        result = static_cast<int>(value.toFloat());

    return result;
}

void Settings::setFps(int value)
{
    QSettings().setValue("LiveView/Fps", value);
}

} // namespace LiveView
