#include "MainWindow.h"
#include <QApplication>
#include <AstroBase/AstroBase>

#include "Sender.h"
#include "StatusPoller.h"

#define xstr(a) str(a)
#define str(a) #a
#define PLUGIN_DIR xstr(ASTRO_INSTALL_PLUGINS)


int main(int argc, char *argv[])
{

    QString host { "192.168.122.1" };
    int port { 8080 };
    int i = 1;
    while(i<argc)
    {
        QString arg { argv[i] };
        if(arg == "--host")
        {
            i++;
            host = argv[i];
        }
        else if(arg == "--port")
        {
            i++;
            port = atoi(argv[i]);
        }

        i++;
    }


    QString organizationName = "AstroPhotoHelpers";
    QString appName = "AstroCameraRemote";

    AstroBase::Logging::initLogging(appName);
    QApplication a(argc, argv);

    if(QDir pluginDir { PLUGIN_DIR }; pluginDir.exists())
        a.addLibraryPath(pluginDir.path());

    a.setOrganizationName(organizationName);
    a.setApplicationName(appName);

    StatusPoller::create(&a);
    Sender* sender { Sender::create(&a) };
    sender->setHost(host);
    sender->setPort(port);

    MainWindow w;
    w.show();

    return a.exec();
}
