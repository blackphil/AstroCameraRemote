#ifndef FITS_IMAGEIOHANDLER_H
#define FITS_IMAGEIOHANDLER_H

#include "fits_global.h"
#include <QImageIOHandler>

namespace Fits {

class ImageIOHandler : public QImageIOHandler
{
public:
    ImageIOHandler();

    bool canRead() const;
    bool read(QImage* image);
};

} // namespace Fits

#endif // FITS_IMAGEIOHANDLER_H
