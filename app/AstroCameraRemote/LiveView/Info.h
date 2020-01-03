#ifndef LIVEVIEW_INFO_H
#define LIVEVIEW_INFO_H

#include <QString>


namespace LiveView {

class Info
{
    int fps;
    QString htmlPattern;
    long frameCount;

public:
    Info();
    int getFps() const;
    void setFps(int value);

    QString toHtml() const;
    long getFrameCount() const;
    void setFrameCount(long value);
};

} // namespace LiveView

#endif // LIVEVIEW_INFO_H
