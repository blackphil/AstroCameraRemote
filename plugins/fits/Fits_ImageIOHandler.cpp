#include "Fits_ImageIOHandler.h"

#include "Fits_File.h"

#include <QDataStream>

namespace Fits {


ImageIOHandler::ImageIOHandler()
{

}

bool ImageIOHandler::canRead() const
{
    return File::isValid(device());
}

bool ImageIOHandler::read(QImage* image)
{
    if(!device())
        return false;

    File file = File::read(device());

    int h = file.height();
    int w = file.width();

    *image = QImage(w, h, QImage::Format_RGB888);

    const QByteArray& data = file.getData();
    QDataStream strm(data);

    qint16 pixel = 0;
    int index = 0;
    while(!strm.atEnd())
    {
        int y = index / w;
        int x = index % w;
        index++;

        strm >> pixel;
        pixel %= 256;
        image->setPixel(x, y, qRgb(pixel, pixel, pixel));
    }

    return true;
}
} // namespace Fits
