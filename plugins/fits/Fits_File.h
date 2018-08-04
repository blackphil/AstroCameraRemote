#ifndef FITS_FILE_H
#define FITS_FILE_H

#include "fits_global.h"

#include <QList>
#include <QPair>
#include <QString>
#include <QByteArray>
#include <QCoreApplication>
#include <QPixmap>

class QIODevice;

namespace Fits {

class File
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

private :
    PixelFormat pixelFormat;
    ColorFormat colorFormat;

    double minVal[3];
    double maxVal[3];

public :

    auto getMinVal(int c) const { return minVal[c]; }
    auto getMaxVal(int c) const { return maxVal[c]; }

    struct HeaderData
    {
        typedef QMap<QString, QString> Attributes;
        Attributes attr;
        QStringList lines;

        int getIntAttr(const QString& key) const;
    };

    static File read(const QByteArray& data);
    static File read(QIODevice* fd);
    static File read(const QString& filePath);

    void write(QIODevice* fd);
    void write(const QString& filePath);

    int width() const;
    int height() const;
    int bitPix() const;

    const QByteArray& getData() const;

    static bool isValid(QIODevice* fd);

    PixelFormat getPixelFormat() const;

    ColorFormat getColorFormat() const;

private :
    QString fileName;
    QString path;
    HeaderData header;
    QByteArray data;
};

} // namespace Fits

#endif // FITS_FILE_H
