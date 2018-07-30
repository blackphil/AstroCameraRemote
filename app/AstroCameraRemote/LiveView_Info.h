#ifndef LIVEVIEW_INFO_H
#define LIVEVIEW_INFO_H

#include <QString>


namespace LiveView {

class Info
{
    float fps;
    QString htmlPattern;
    long frameCount;

public:
    Info();
    float getFps() const;
    void setFps(float value);

    QString toHtml() const;
    long getFrameCount() const;
    void setFrameCount(long value);
};

} // namespace LiveView

#endif // LIVEVIEW_INFO_H
