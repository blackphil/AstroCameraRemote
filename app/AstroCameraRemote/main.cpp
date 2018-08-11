#include "MainWindow.h"
#include <QApplication>

#define xstr(a) str(a)
#define str(a) #a
#define PLUGIN_DIR xstr(ASTRO_INSTALL_PLUGINS)


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString pluginDir = PLUGIN_DIR;
    if(QDir(pluginDir).exists())
        a.addLibraryPath(pluginDir);

    a.setOrganizationName("AstroPhotoHelpers");

    MainWindow w;
    w.show();

    return a.exec();
}
