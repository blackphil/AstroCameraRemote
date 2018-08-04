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

private :
    QString fileName;
    QString path;
    HeaderData header;
    QByteArray data;
};

} // namespace Fits

#endif // FITS_FILE_H
