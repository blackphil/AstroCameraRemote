#include "MainWindow.h"
#include <QApplication>

//#include "Fits_File.h"
#include "AstroBase_Exception.h"
int main(int argc, char *argv[])
{

    AstroBase::Exception e("blub");
//    Fits::File f;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
