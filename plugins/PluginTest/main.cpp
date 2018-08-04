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

void testFile(const QString& in, const QString& out, const QString& msg)
{
    AB_INF(msg);
    AB_INF("read:" << in);
    QFile f(in);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    QImage img = QImage::fromData(data);

    AB_INF("write as jpeg:" << out);
    img.save(QFileInfo(QDir::temp(), out).absoluteFilePath(), "JPG");
}

int main(int argc, char *argv[])
{
    AB_DBG("BEGIN");
    QCoreApplication a(argc, argv);

    try {

        a.addLibraryPath(PLUGIN_DIR);

        testFile(":/jpg/DSC03274.JPG"                    , "DSC03274_generated.JPG"         , "test reading jpeg");
        testFile(":/fits/Capture_4_0_g_0895_12b.fit"     , "Capture_4_0_g_0895_12b.jpg"     , "test reading fits (bayer RGGB)");
        testFile(":/fits/Capture_4_0_g_0895_12b_0001.FIT", "Capture_4_0_g_0895_12b_0001.jpg", "test reading fits (RGB16)");




    } catch (std::exception& e) {
        AB_ERR(e.what());
    }
    AB_DBG("END");
    return 0;

}
