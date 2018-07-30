#include "LiveView_Info.h"

#include <QFile>

namespace LiveView {

long Info::getFrameCount() const
{
    return frameCount;
}

void Info::setFrameCount(long value)
{
    frameCount = value;
}

Info::Info()
    : fps(0)
{

    QFile htmlPatternFile(":/sequencer/LiveView_Info.html");
    htmlPatternFile.open(QIODevice::ReadOnly | QIODevice::Text);
    htmlPattern = htmlPatternFile.readAll();
}

float Info::getFps() const
{
    return fps;
}

void Info::setFps(float value)
{
    fps = value;
}

QString Info::toHtml() const
{

    return htmlPattern.arg(fps, 0, 'g', 2).arg(frameCount);
}


} // namespace LiveView
