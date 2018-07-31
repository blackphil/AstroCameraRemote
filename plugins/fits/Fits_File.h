#ifndef FITS_FILE_H
#define FITS_FILE_H

#include <QList>
#include <QPair>
#include <QString>
#include <QByteArray>
#include <QCoreApplication>

class QIODevice;

namespace Fits {

class File
{
    Q_DECLARE_TR_FUNCTIONS(Fits::File)
public:

    typedef QList<QPair<QString, QString> > HeaderData;

    static File read(QIODevice* fd);
    static File read(const QString& filePath);

    void write(QIODevice* fd);
    void write(const QString& filePath);

private :
    QString fileName;
    QString path;
    HeaderData header;
    QByteArray data;
};

} // namespace Fits

#endif // FITS_FILE_H
