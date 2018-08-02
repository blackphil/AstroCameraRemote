#include "Fits_ImageIOHandler.h"

#include "Fits_ImageIOPlugin.h"
#include "Fits_File.h"

namespace Fits {

ImageIOHandler::ImageIOHandler()
{

}

bool ImageIOHandler::canRead() const
{
    return ImageIOPlugin().capabilities(this->device(), this->format()) & QImageIOPlugin::CanRead;
}

bool ImageIOHandler::read(QImage* image)
{
    if(!device())
        return false;

    File file = File::read(device());

    uint w = file.width();
    uint h = file.height();

    uint bytesPerPixel = static_cast<uint>(file.bitPix()) / 8;
    const char* data = file.getData().data();
    uchar* values = new uchar[bytesPerPixel];
    for(uint y=0; y<h; y++)
    {
        for(uint x=0; x<h; x++)
        {
            uint index = bytesPerPixel * ((y * w) + x);
            int value = 0;
            for(uint b=0; b<bytesPerPixel; b++)
            {
                values[b] = data[index+b];
                value += values[b] << ((bytesPerPixel-1-b)*8);
            }

            value %= 256;



            image->setPixel(x, y, qRgb(value, value, value));
        }
    }
    delete[] values;

    return true;
}
} // namespace Fits
