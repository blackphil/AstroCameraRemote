#include "PostView_ImageLoader.h"

#include <QFile>
#include <QImage>
#include <QByteArray>

#include "AstroBase.h"

namespace PostView {

ImageLoader::ImageLoader(const QString &fileName, QPixmap *pm)
    : pm(pm)
    , fileName(fileName)
{

}

void ImageLoader::run()
{
    try {
        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        QByteArray data = f.readAll();
        *pm = QPixmap::fromImage(QImage::fromData(data));

        Q_EMIT finished();

    } catch (std::exception& e) {
        AB_ERR(e.what());
    }
}

} // namespace PostView
