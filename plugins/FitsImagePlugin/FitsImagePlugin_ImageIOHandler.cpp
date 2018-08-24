#include "FitsImagePlugin_ImageIOHandler.h"
#include "AstroBase.h"
#include "AstroBase_Exception.h"
#include "Fits_File.h"

#include <QDataStream>

namespace Fits {


bool ImageIOHandler::read(FilePtr f, QImage *image)
{
    switch(f->getPixelFormat())
    {
    case File::PixelFormat_16Bit_Int :
    {
        if(f->getColorFormat() == File::ColorFormat_BayerRGGB)
            return read16BitIntBayer(f, image);
        else if(f->getColorFormat() != File::ColorFormat_Grayscale)
            throw AstroBase::Exception(tr("Pixel format of 16bit integer and non-grayscale not supported for now."));

        return read16BitInt(f, image);
    }
    case File::PixelFormat_32Bit_Single :
    {
        if(f->getColorFormat() != File::ColorFormat_RGB)
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

bool ImageIOHandler::readRGBFloat(FilePtr file, QImage *image)
{
    int h = file->height();
    int w = file->width();

    *image = QImage(w, h, QImage::Format_RGB888);

    const QByteArray& data = file->getData();
    QDataStream strm(data);

    int numPixels = w*h;
    QVector<float> pixmap[3] = { QVector<float>(numPixels), QVector<float>(numPixels), QVector<float>(numPixels) };
    float minVal = std::numeric_limits<float>::max();
    float maxVal = std::numeric_limits<float>::min();
    float delta = 0;
    int pixelIndex = 0;
    for(int c=0; c<3; c++)
    {
        for(int i=0; i<numPixels; i++)
        {
            float pixel = float(file->getPixel(pixelIndex));
            pixmap[c][i] = pixel;
            minVal = qMin(minVal, pixel);
            maxVal = qMax(maxVal, pixel);

            pixelIndex++;
        }
    }
    delta = maxVal - minVal;


    for(int i=0; i<numPixels; i++)
    {
        int x = i % w;
        int y = i / w;
        int pixel[3] = { 0, 0, 0 };
        for(int c=0; c<3; c++)
        {
            const float& pixelF = pixmap[c][i];
            pixel[c] = int(((pixelF-minVal) / delta) * 256.f);
        }

        image->setPixel(x, y, qRgb(pixel[0], pixel[1], pixel[2]));
    }


    return true;
}

bool ImageIOHandler::read16BitInt(FilePtr file, QImage *image)
{
    int h = file->height();
    int w = file->width();

    *image = QImage(w, h, QImage::Format_Grayscale8);

    for(int i=0; i<file->numPixels(); i++)
    {
        int y = i / w;
        int x = i % w;

        qint16 pixel = qint16(file->getPixel(i));
        image->setPixel(x, y, qRgb(pixel, pixel, pixel));
    }

    return true;
}

bool ImageIOHandler::read16BitIntBayer(FilePtr file, QImage *image)
{
    auto h = file->height();
    auto w = file->width();

    *image = QImage(w/2, h/2, QImage::Format_RGB888);


    int row = 0;
    int col = 0;

    QVector<qint16> pixels[3] = { QVector<qint16>((w*h)/4), QVector<qint16>((w*h)/4), QVector<qint16>((w*h)/4) };

    const char* bayerMask = file->getBayerMask();

    int pixelIndex = 0;

    qint16 maxVal = std::numeric_limits<qint16>::min();
    qint16 minVal = std::numeric_limits<qint16>::max();
    qint16 delta = 0;

    for(int index=0; index<file->numPixels(); index++)
    {
        row = index / w;
        col = index % w;

        qint16 pixel = qint16(file->getPixel(index));

        int c = ((row%2) << 1) | (col%2);
        c = bayerMask[c];

        if(c==1)
            pixels[c][pixelIndex] += pixel/2;
        else
            pixels[c][pixelIndex] = pixel;

        maxVal = qMax(maxVal, pixels[c][pixelIndex]);
        minVal = qMin(minVal, pixels[c][pixelIndex]);
        delta = maxVal-minVal;

        if(col%2==1)
        {
            if(col==(w-1) && row%2==0)
                pixelIndex-=w/2;

            pixelIndex++;
        }
    }

    for(auto i=0; i<((w*h)/4); i++)
    {
        int x = i % (w/2);
        int y = i / (w/2);
        image->setPixel(
                    x, y
                    , qRgb(
                        static_cast<int>((pixels[0][i] * 256) / delta)
                      , static_cast<int>((pixels[1][i] * 256) / delta)
                      , static_cast<int>((pixels[2][i] * 256) / delta)));
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

    FilePtr file = File::fromDevice(device());

    return read(file, image);
}
} // namespace Fits
