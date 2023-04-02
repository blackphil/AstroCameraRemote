#include "MainWindow.h"
#include <QApplication>

#include <AstroBase/AstroBase>
#include <AstroBase/Exception>

int main(int argc, char *argv[])
{
    QString organizationName = "AstroPhotoHelpers";
    QString appName = "AstroImageStacker";

    AstroBase::Logging::initLogging(appName);
    QApplication a(argc, argv);
    a.setOrganizationName(organizationName);
    a.setApplicationName(appName);

    MainWindow w;
    w.show();
    return a.exec();
}
