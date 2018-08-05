#ifndef FITS_IMAGEIOHANDLER_H
#define FITS_IMAGEIOHANDLER_H

#include "fits_global.h"
#include <QImageIOHandler>
#include <QCoreApplication>

namespace Fits {

class File;

class ImageIOHandler : public QImageIOHandler
{
    Q_DECLARE_TR_FUNCTIONS(Fits::ImageIOHandler)

    bool read(const File& f, QImage* image);
    bool readRGBFloat(const File& file, QImage* image);
    bool read16BitInt(const File& file, QImage* image);
    bool read16BitIntBayer(const File& file, QImage* image);

public:
    ImageIOHandler();

    bool canRead() const;
    bool read(QImage* image);
};

} // namespace Fits

#endif // FITS_IMAGEIOHANDLER_H
