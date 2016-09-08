#ifndef SETTINGS_GENERAL_H
#define SETTINGS_GENERAL_H

#include "SonyAlphaRemote_Settings.h"


namespace Settings {

class General : public SonyAlphaRemote::Setting
{
    static QString groupName;
    static bool lenrEnabledDefault;
    static QString lenrEnabledName;

    bool lenrEnabled;

public:
    General(Setting* parent);

    void save();
    void load();


    bool getLenrEnabled() const;
    void setLenrEnabled(bool value);
};

} // namespace Settings

#endif // SETTINGS_GENERAL_H
