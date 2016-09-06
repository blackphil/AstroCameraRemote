#ifndef SONYALPHAREMOTE_POSTVIEW_INFO_H
#define SONYALPHAREMOTE_POSTVIEW_INFO_H

#include <QDateTime>
#include <QUrl>
#include <QPixmap>

namespace SonyAlphaRemote {
namespace PostView {

class Info
{
    int shutterSpeedMs;
    int iso;
    QDateTime timestamp;
    QUrl url;
    int seqNr;
    int numShots;
    QPixmap image;


public:
    Info();
    Info(int shutterSpeedMs, int iso, const QDateTime& ts, const QUrl& url, int seqNr = 0, int numShots = 0, QPixmap image = QPixmap());
    int getShutterSpeedMs() const;
    void setShutterSpeedMs(int value);
    int getIso() const;
    void setIso(int value);
    QDateTime getTimestamp() const;
    void setTimestamp(const QDateTime &value);
    QUrl getUrl() const;
    void setUrl(const QUrl &value);
    int getSeqNr() const;
    void setSeqNr(int value);
    int getNumShots() const;
    void setNumShots(int value);
    const QPixmap& getImage() const;
    void setImage(const QPixmap &value);
};

} // namespace PostView
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_POSTVIEW_INFO_H
