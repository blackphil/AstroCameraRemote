#include "PostView_Info.h"

#include <QFile>

namespace PostView {

QString Info::htmlPattern;

QString Info::getShutterSpeed() const
{
    return shutterSpeed;
}

void Info::setShutterSpeed(const QString &value)
{
    shutterSpeed = value;
}

int Info::getShutterSpeedBulbMs() const
{
    return shutterSpeedBulbMs;
}

void Info::setShutterSpeedBulbMs(int value)
{
    shutterSpeedBulbMs = value;
}

Info::Info()
    : shutterSpeedBulbMs(0)
    , iso(0)
    , seqNr(0)
    , numShots(0)
{
    if(htmlPattern.isEmpty())
    {
        QFile htmlPatternFile(":/sequencer/PostView_Info.html");
        htmlPatternFile.open(QIODevice::ReadOnly | QIODevice::Text);
        htmlPattern = htmlPatternFile.readAll();
    }
}

Info::Info(
        QString shutterSpeed, int shutterSpeedBulbMs, int iso
        , const QDateTime& ts, const QUrl& url, int seqNr, int numShots
        , const QPixmap &image)
    : shutterSpeed(shutterSpeed)
    , shutterSpeedBulbMs(shutterSpeedBulbMs)
    , iso(iso)
    , timestamp(ts)
    , url(url)
    , seqNr(seqNr)
    , numShots(numShots)
    , image(image)
{

}



int Info::getIso() const
{
    return iso;
}

void Info::setIso(int value)
{
    iso = value;
}

QDateTime Info::getTimestamp() const
{
    return timestamp;
}

void Info::setTimestamp(const QDateTime &value)
{
    timestamp = value;
}

QUrl Info::getUrl() const
{
    return url;
}

void Info::setUrl(const QUrl &value)
{
    url = value;
}

int Info::getSeqNr() const
{
    return seqNr;
}

void Info::setSeqNr(int value)
{
    seqNr = value;
}

int Info::getNumShots() const
{
    return numShots;
}

void Info::setNumShots(int value)
{
    numShots = value;
}

const QPixmap &Info::getImage() const
{
    return image;
}

void Info::setImage(const QPixmap &value)
{
    image = value;
}

QString Info::toHtml() const
{
    QString shutterSpeedStr = shutterSpeed;
    if(shutterSpeed == "BULB")
        shutterSpeedStr = QString("%0ms (BULB)").arg(shutterSpeedBulbMs);

    return htmlPattern
            .arg(timestamp.toString("yyyy-MM-dd"))
            .arg(timestamp.toString("HH:mm:ss:zzz"))
            .arg(seqNr)
            .arg(numShots)
            .arg(shutterSpeedStr)
            .arg(iso)
            .arg(image.isNull() ? "?" : QString::number(image.width()))
            .arg(image.isNull() ? "?" : QString::number(image.height()));
}


} // namespace PostView
