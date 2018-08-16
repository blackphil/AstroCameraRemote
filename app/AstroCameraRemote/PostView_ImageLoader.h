#ifndef POSTVIEW_IMAGELOADER_H
#define POSTVIEW_IMAGELOADER_H

#include <QRunnable>
#include <QObject>
#include <QString>
#include <QPixmap>

namespace PostView {

class ImageLoader : public QObject, public QRunnable
{
    Q_OBJECT

    QPixmap* pm;
    QString fileName;

public:
    ImageLoader(const QString& fileName, QPixmap* pm);

    void run();

Q_SIGNALS :
    void finished();
};

} // namespace PostView

#endif // POSTVIEW_IMAGELOADER_H
