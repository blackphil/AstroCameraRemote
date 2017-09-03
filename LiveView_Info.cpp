#include "LiveView_Info.h"

#include <QFile>

namespace LiveView {

Info::Info()
    : fps(0)
{

    QFile htmlPatternFile(":/sequencer/LiveView_Info.html");
    htmlPatternFile.open(QIODevice::ReadOnly | QIODevice::Text);
    htmlPattern = htmlPatternFile.readAll();
}

int Info::getFps() const
{
    return fps;
}

void Info::setFps(int value)
{
    fps = value;
}

QString Info::toHtml() const
{

    return htmlPattern.arg(fps);
}


} // namespace LiveView
