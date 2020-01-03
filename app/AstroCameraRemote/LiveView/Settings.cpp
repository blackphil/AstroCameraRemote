#include "Settings.h"

#include <QSettings>

#include "../Helper.h"

namespace LiveView {

int Settings::getFps()
{
    QVariant value { QSettings{}.value("LiveView/Fps", 10) };

    bool ok { false };
    int result = value.toInt(&ok);
    if(!ok)
        result = static_cast<int>(value.toFloat());

    return result;
}

void Settings::setFps(int value)
{
    QSettings{}.setValue("LiveView/Fps", value);
}

} // namespace LiveView
