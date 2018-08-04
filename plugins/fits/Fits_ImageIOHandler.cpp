#include "Fits_ImageIOHandler.h"
#include "AstroBase.h"
#include "AstroBase_Exception.h"
#include "Fits_File.h"

#include <QDataStream>

namespace Fits {


bool ImageIOHandler::read(const File &f, QImage *image)
{
    switch(f.getPixelFormat())
    {
    case File::PixelFormat_16Bit_Int :
    {
        if(f.getColorFormat() == File::ColorFormat_BayerRGGB)
            AB_WRN("Bayer not supported ... handle as grayscale for now");
        else if(f.getColorFormat() != File::ColorFormat_Grayscale)
            throw AstroBase::Exception(tr("Pixel format of 16bit integer and non-grayscale not supported for now."));

        return read16BitInt(f, image);
    }
    case File::PixelFormat_32Bit_Single :
    {
        if(f.getColorFormat() != File::ColorFormat_RGB)
            throw AstroBase::Exception(tr("32 floating point pixels + RGB supported only for now!"));
        return readRGBFloat(f, image);
    }
    case File::PixelFormat_32Bit_Int :
    case File::PixelFormat_64Bit_Double :
    case File::PixelFormat_64Bit_Int :
    default:
        break;
    }
    throw AstroBase::Exception(tr("unsupported pixel format."));
}

bool ImageIOHandler::readRGBFloat(const File &file, QImage *image)
{
    int h = file.height();
    int w = file.width();

    *image = QImage(w, h, QImage::Format_RGB888);

    const QByteArray& data = file.getData();
    QDataStream strm(data);

    int numPixels = w*h;
    QVector<float> pixmap[3] = { QVector<float>(numPixels), QVector<float>(numPixels), QVector<float>(numPixels) };
    float minVal[3] = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    float maxVal[3] = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };
    float distance[3] = { 0, 0, 0};

    for(int c=0; c<3; c++)
    {
        for(int i=0; i<numPixels; i++)
        {
            float pixel = 0.f;
            uchar bytes[4];
            strm >> bytes[3] >> bytes[2] >> bytes[1] >> bytes[0];
            memcpy(&pixel, &bytes, sizeof(float));
            //                pixel = qBound(0.f, pixel, 5000.f);
            pixmap[c][i] = pixel;
            minVal[c] = qMin(minVal[c], pixel);
            maxVal[c] = qMax(maxVal[c], pixel);
        }
        distance[c] = maxVal[c] - minVal[c];
    }


    for(int i=0; i<numPixels; i++)
    {
        int x = i % w;
        int y = i / w;
        int pixel[3] = { 0, 0, 0 };
        for(int c=0; c<3; c++)
        {
            const float& pixelF = pixmap[c][i];
            pixel[c] = static_cast<int>((pixelF / distance[c]) * 256.f);
        }

        image->setPixel(x, y, qRgb(pixel[0], pixel[1], pixel[2]));
    }


    return true;
}

bool ImageIOHandler::read16BitInt(const File &file, QImage *image)
{
    int h = file.height();
    int w = file.width();

    *image = QImage(w, h, QImage::Format_Grayscale8);

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

    return read(file, image);
}
} // namespace Fits
