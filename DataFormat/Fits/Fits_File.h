#ifndef FITS_FILE_H
#define FITS_FILE_H

#include "fits_global.h"

#include <QSharedPointer>
#include <QList>
#include <QPair>
#include <QString>
#include <QByteArray>
#include <QCoreApplication>
#include <QPixmap>

class QIODevice;

namespace Fits {

static const int HeaderLineSize = 80;
static const int HeaderLineCount = 36;


class File;
typedef QSharedPointer<File> FilePtr;

class FITSSHARED_EXPORT File
{
    Q_DECLARE_TR_FUNCTIONS(Fits::File)
public:

    enum PixelFormat
    {
        PixelFormat_8Bit = 8
        , PixelFormat_16Bit_Int = 16
        , PixelFormat_32Bit_Int = 32
        , PixelFormat_64Bit_Int = 64
        , PixelFormat_32Bit_Single = -32
        , PixelFormat_64Bit_Double = -64
    };

    enum ColorFormat
    {
        ColorFormat_Grayscale
        , ColorFormat_BayerRGGB
        , ColorFormat_RGB
    };

    struct HeaderData
    {
        typedef QMap<QString, QString> Attributes;
        Attributes attr;
        QStringList lines;

        int getIntAttr(const QString& key) const;
    };

private :
    QString filePath;
    HeaderData header;
    QByteArray data;

    PixelFormat pixelFormat;
    ColorFormat colorFormat;

    bool loaded;

    char bayerMask[4];
    double minVal[3];
    double maxVal[3];

    void init();

public :

    File();
    File(int w, int h, PixelFormat pf, ColorFormat cf);
    File(const QString& filePath, bool loadNow);

    bool load();
    bool load(QIODevice* device);
    bool loadHeader();
    bool loadHeader(QIODevice* fd);
    bool isHeaderLoaded() const;
    bool isLoaded() const { return loaded; }

    const char* getBayerMask() const { return bayerMask; }

    auto getMinVal(int c) const { return minVal[c]; }
    auto getMaxVal(int c) const { return maxVal[c]; }


    static FilePtr fromByteArray(const QByteArray& data);
    static FilePtr fromDevice(QIODevice* fd);
    static FilePtr fromFile(const QString& filePath);

    void save();
    void save(QIODevice* fd) const;
    void save(QByteArray& ba);
    void save(const QString& filePath);

    void saveAs(const QString& filePath) const;

    int width() const;
    int height() const;
    int bitPix() const;
    int numPixels() const;

    double getPixel(int pixelIndex) const;
    void setPixel(int pixelIndex, const double& value);

    const QByteArray& getData() const;

    static bool isValid(QIODevice* fd);

    PixelFormat getPixelFormat() const;
    ColorFormat getColorFormat() const;

    QString getFilePath() const;
    void setFilePath(const QString &value);
};

} // namespace Fits

#endif // FITS_FILE_H
