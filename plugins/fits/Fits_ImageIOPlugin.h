#ifndef FITS_IMAGEIOPLUGIN_H
#define FITS_IMAGEIOPLUGIN_H

#include "fits_global.h"
#include <QImageIOPlugin>

namespace Fits {

class FITSSHARED_EXPORT ImageIOPlugin : public QImageIOPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.AstroImage.Fits.ImageIOPlugin" FILE "FitsMetadata.json")
public:
    ImageIOPlugin();

    QImageIOPlugin::Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler* create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

} // namespace Fits

#endif // FITS_IMAGEIOPLUGIN_H
