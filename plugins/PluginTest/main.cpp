#include <QCoreApplication>

#include <exception>

#include <QImageReader>
#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "AstroBase.h"
#include "Fits_ImageIOPlugin.h"

#define xstr(a) str(a)
#define str(a) #a
#define PLUGIN_DIR xstr(ASTRO_INSTALL_PLUGINS)

int main(int argc, char *argv[])
{
    AB_DBG("BEGIN");
    QCoreApplication a(argc, argv);

    try {

        a.addLibraryPath(PLUGIN_DIR);

        QList<QByteArray> list = QImageReader::supportedImageFormats();

        QFile testJpgFile(":/jpg/DSC03274.JPG");
        testJpgFile.open(QIODevice::ReadOnly);
        QByteArray data = testJpgFile.readAll();

        AB_INF("read jpeg ...");
        QImage testJpg = QImage::fromData(data);

        AB_INF("save jpeg ...");
        testJpg.save(QFileInfo(QDir::temp(), "DSC03274_generated.JPG").absoluteFilePath(), "JPG");

        QFile testFitsFile(":/fits/Capture_4_0_g_0895_12b.fit");
        testFitsFile.open(QIODevice::ReadOnly);
        data = testFitsFile.readAll();

        AB_INF("read fits ...");
        QImage testFits = QImage::fromData(data);

        AB_INF("save fits as jpeg ...");
        testFits.save(QFileInfo(QDir::temp(), "Capture_4_0_g_0895_12b.jpg").absoluteFilePath(), "JPG");



    } catch (std::exception& e) {
        AB_ERR(e.what());
    }
    AB_DBG("END");
    return 0;

}
