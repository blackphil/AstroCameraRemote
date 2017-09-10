#include "Settings_General.h"
#include "SonyAlphaRemote_Settings.h"

namespace Settings {


bool General::getLenrEnabled()
{
    return QSettings().value("Camera/lenrEnabled", false).toBool();
}

void General::setLenrEnabled(bool value)
{
    QSettings().setValue("Camera/lenrEnabled", value);
}


} // namespace Settings
