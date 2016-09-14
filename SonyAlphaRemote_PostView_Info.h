#ifndef SONYALPHAREMOTE_POSTVIEW_INFO_H
#define SONYALPHAREMOTE_POSTVIEW_INFO_H

#include <QDateTime>
#include <QUrl>
#include <QPixmap>

namespace SonyAlphaRemote {
namespace PostView {

class Info
{
    QString shutterSpeed;
    int shutterSpeedBulbMs;
    int iso;
    QDateTime timestamp;
    QUrl url;
    int seqNr;
    int numShots;
    QPixmap image;

    static QString htmlPattern;


public:
    Info();
    Info(
            QString shutterSpeed, int shutterSpeedBulbMs, int iso
            , const QDateTime &ts, const QUrl &url, int seqNr, int numShots
            , const QPixmap& image = QPixmap());
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

    QString toHtml() const;
    QString getShutterSpeed() const;
    void setShutterSpeed(const QString &value);
    int getShutterSpeedBulbMs() const;
    void setShutterSpeedBulbMs(int value);
};

} // namespace PostView
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_POSTVIEW_INFO_H
