#ifndef PLUGINMANAGER_IIMAGEFILEPLUGIN_H
#define PLUGINMANAGER_IIMAGEFILEPLUGIN_H

#include <QStringList>


namespace PluginManager {

class IImageFilePlugin
{
public:

    virtual ~IImageFilePlugin() {}

    virtual QStringList supportedExtensions() const = 0;
    virtual int processFile(const QString& inputFilePath, const QString& outputFilePath) = 0;
};

} // namespace PluginManager

#endif // PLUGINMANAGER_IIMAGEFILEPLUGIN_H
