#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QImageReader>

#include "Fits_ImageIOPlugin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("AstroPhotoHelpers");



    QList<QByteArray> list = QImageReader::supportedImageFormats();

    QDir pluginsDir = QCoreApplication::applicationDirPath();
    pluginsDir.cd("plugins");
    pluginsDir.cd("imageformats");

    QPluginLoader loader("F:/philipp/dev/build-AstroImage-Qt5_11_MSVC2017-Debug/plugins/fits/debug/fits");
    QObject* plugin = loader.instance();

    Fits::ImageIOPlugin* fitsPlugin = dynamic_cast<Fits::ImageIOPlugin*>(plugin);
    if(fitsPlugin)
    {
        QFile fitsFile("F:/philipp/capture_PLx/20170430/light/Capture_4_2_g_0895_12b.fit");
        fitsFile.open(QIODevice::ReadOnly);
        QImageIOHandler* ioHandler = fitsPlugin->create(&fitsFile, "FITS");

        QImage fitsImage(800, 600, QImage::Format_RGB16);
        ioHandler->read(&fitsImage);
    }

    list = QImageReader::supportedImageFormats();

    MainWindow w;
    w.show();

    return a.exec();
}
