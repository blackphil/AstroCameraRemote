#ifndef FITS_IMAGEIOHANDLER_H
#define FITS_IMAGEIOHANDLER_H

#include "fits_global.h"
#include <QImageIOHandler>
#include <QCoreApplication>
#include <QSharedPointer>

namespace Fits {

class File;
typedef QSharedPointer<File> FilePtr;

class ImageIOHandler : public QImageIOHandler
{
    Q_DECLARE_TR_FUNCTIONS(Fits::ImageIOHandler)

    bool read(FilePtr f, QImage* image);
    bool readRGBFloat(FilePtr file, QImage* image);
    bool read16BitInt(FilePtr file, QImage* image);
    bool read16BitIntBayer(FilePtr file, QImage* image);

public:
    ImageIOHandler();

    bool canRead() const;
    bool read(QImage* image);
};

} // namespace Fits

#endif // FITS_IMAGEIOHANDLER_H
