#ifndef FITS_REPAIR_H
#define FITS_REPAIR_H

#include "PluginManager_IImageFilePlugin.h"

namespace Fits {

class Repair : public PluginManager::IImageFilePlugin
{
public:
    Repair();

    QStringList supportedExtensions() const;
    int processFile(const QString& inputFilePath, const QString& outputFilePath);
};

} // namespace Fits

#endif // FITS_REPAIR_H
