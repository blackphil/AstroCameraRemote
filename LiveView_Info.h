#ifndef LIVEVIEW_INFO_H
#define LIVEVIEW_INFO_H

#include <QString>


namespace LiveView {

class Info
{
    int fps;
    QString htmlPattern;

public:
    Info();
    int getFps() const;
    void setFps(int value);

    QString toHtml() const;
};

} // namespace LiveView

#endif // LIVEVIEW_INFO_H
