#include "SonyAlphaRemote_PostView_Info.h"

namespace SonyAlphaRemote {
namespace PostView {


Info::Info()
    : shutterSpeedMs(0)
    , iso(0)
    , seqNr(0)
    , numShots(0)
{

}

Info::Info(int shutterSpeedMs, int iso, const QDateTime& ts, const QUrl& url, int seqNr, int numShots, QPixmap image)
    : shutterSpeedMs(shutterSpeedMs)
    , iso(iso)
    , timestamp(ts)
    , url(url)
    , seqNr(seqNr)
    , numShots(numShots)
    , image(image)
{

}

int Info::getShutterSpeedMs() const
{
    return shutterSpeedMs;
}

void Info::setShutterSpeedMs(int value)
{
    shutterSpeedMs = value;
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


} // namespace PostView
} // namespace SonyAlphaRemote
