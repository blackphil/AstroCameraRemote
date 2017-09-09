#ifndef SETTINGS_GENERAL_H
#define SETTINGS_GENERAL_H


#include <SonyAlphaRemote_Settings.h>

namespace Settings {

class General : public SonyAlphaRemote::Setting
{
    Q_OBJECT

    static QString groupName;
    static bool lenrEnabledDefault;
    static QString lenrEnabledName;

    bool lenrEnabled;

public:
    General(Setting* parent);

    static QString getName();

    void save();
    void load();


    bool getLenrEnabled() const;
    void setLenrEnabled(bool value);

    static General* getInstance();
};

} // namespace Settings

#endif // SETTINGS_GENERAL_H
