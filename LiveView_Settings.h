#ifndef LIVEVIEW_SETTINGS_H
#define LIVEVIEW_SETTINGS_H

#include "SonyAlphaRemote_Settings.h"

namespace LiveView {

class Settings : public SonyAlphaRemote::Setting
{
    Q_OBJECT

    float fps;

public:
    Settings(Setting* parent);
    float getFps() const;
    void setFps(float value);

    void load();
    void save();

    static QString getName();
};

} // namespace LiveView

#endif // LIVEVIEW_SETTINGS_H
