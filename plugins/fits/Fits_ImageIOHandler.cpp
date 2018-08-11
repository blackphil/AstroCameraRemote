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
            return read16BitIntBayer(f, image);
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

bool ImageIOHandler::read16BitIntBayer(const File &file, QImage *image)
{
    auto h = file.height();
    auto w = file.width();

    *image = QImage(w/2, h/2, QImage::Format_RGB888);

    const QByteArray& data = file.getData();


    auto expectedDataSize = w * h * 2/*for 16bit integer*/;
    auto dataSize = data.size();
    Q_ASSERT(dataSize == expectedDataSize);

    auto numValues = expectedDataSize / 2;

    QDataStream strm(data);

    typedef qint16 Pixel16;
    typedef qint32 Pixel32;

    Pixel16 pixel16 = 0;
    Pixel32 pixel32 = 0;
    int row = 0;
    int col = 0;
    int index = 0;

    QVector<Pixel32> pixels[3] = { QVector<Pixel32>((w*h)/4), QVector<Pixel32>((w*h)/4), QVector<Pixel32>((w*h)/4) };

    const char* bayerMask = file.getBayerMask();

    int pixelIndex = 0;

//    Pixel32 maxVal[3] = { std::numeric_limits<Pixel32>::min(), std::numeric_limits<Pixel32>::min(), std::numeric_limits<Pixel32>::min() };
//    Pixel32 minVal[3] = { std::numeric_limits<Pixel32>::max(), std::numeric_limits<Pixel32>::max(), std::numeric_limits<Pixel32>::max() };
//    Pixel32 delta[3] = { 0, 0, 0 };

    Pixel32 maxVal = std::numeric_limits<Pixel32>::min();
    Pixel32 minVal = std::numeric_limits<Pixel32>::max();
    Pixel32 delta = 0;

    while(!strm.atEnd())
    {
        if(index >= numValues)
            break;

        row = index / w;
        col = index % w;

        strm >> pixel16;
        //pixel32 += static_cast<quint32>(pixel16 + std::numeric_limits<qint16>::max());
        pixel32 = pixel16;

        int c = ((row%2) << 1) | (col%2);
        c = bayerMask[c];

        if(c==1)
            pixels[c][pixelIndex] += pixel32/2;
        else
            pixels[c][pixelIndex] = pixel32;

//        maxVal[c] = qMax(maxVal[c], pixels[c][pixelIndex]);
//        minVal[c] = qMin(minVal[c], pixels[c][pixelIndex]);
//        delta[c] = maxVal[c]-minVal[c];

        maxVal = qMax(maxVal, pixels[c][pixelIndex]);
        minVal = qMin(minVal, pixels[c][pixelIndex]);
        delta = maxVal-minVal;

        if(col%2==1)
        {
            if(col==(w-1) && row%2==0)
                pixelIndex-=w/2;

            pixelIndex++;
        }

        index++;
    }

    for(auto i=0; i<((w*h)/4); i++)
    {
        int x = i % (w/2);
        int y = i / (w/2);

//        image->setPixel(
//                    x, y
//                    , qRgb(
//                        static_cast<int>((pixels[0][i] * 256) / delta[0])
//                      , static_cast<int>((pixels[1][i] * 256) / delta[1])
//                      , static_cast<int>((pixels[2][i] * 256) / delta[2])));
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

    File file = File::fromDevice(device());

    return read(file, image);
}
} // namespace Fits
