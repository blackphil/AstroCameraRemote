#include "Settings_General.h"
#include "SonyAlphaRemote_Settings.h"



bool GeneralSettings::getLenrEnabled()
{
    return QSettings().value("Camera/lenrEnabled", false).toBool();
}

void GeneralSettings::setLenrEnabled(bool value)
{
    QSettings().setValue("Camera/lenrEnabled", value);
}
