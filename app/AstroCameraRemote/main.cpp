#include "MainWindow.h"
#include <QApplication>
#include "AstroBase.h"
#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_StatusPoller.h"

#define xstr(a) str(a)
#define str(a) #a
#define PLUGIN_DIR xstr(ASTRO_INSTALL_PLUGINS)


int main(int argc, char *argv[])
{
    QString organizationName = "AstroPhotoHelpers";
    QString appName = "AstroCameraRemote";

    AstroBase::Logging::initLogging(appName);
    QApplication a(argc, argv);

    QString pluginDir = PLUGIN_DIR;
    if(QDir(pluginDir).exists())
        a.addLibraryPath(pluginDir);

    a.setOrganizationName(organizationName);
    a.setApplicationName(appName);

    StatusPoller::create(&a);
    Sender::create(&a);

    MainWindow w;
    w.show();

    return a.exec();
}
