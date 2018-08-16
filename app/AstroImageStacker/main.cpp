#include "MainWindow.h"
#include <QApplication>


#include "AstroBase_Exception.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("AstroPhotoHelpers");

    MainWindow w;
    w.show();
    return a.exec();
}
