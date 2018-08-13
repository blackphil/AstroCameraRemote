#ifndef BATCHPROCESS_IMAGESIGNAL_H
#define BATCHPROCESS_IMAGESIGNAL_H

#include "batchprocess_global.h"

#include "Fits_File.h"

#include <QList>
#include <QSharedPointer>

namespace BatchProcess {

class ImageSignal;
typedef QSharedPointer<ImageSignal> ImageSignalPtr;

class BATCHPROCESSSHARED_EXPORT ImageSignal
{
    ImageSignal(const ImageSignal& rhs, bool deep = false);
    bool operator=(const ImageSignal& rhs);

    QString name;
    QList<Fits::FilePtr> images;

public:
    ImageSignal(const QString& name);
    virtual ~ImageSignal();

    virtual ImageSignalPtr clone() const;
    int numImages() const;
    const QList<Fits::FilePtr>& getImages() const;
    void setImages(const QList<Fits::FilePtr> &value);
    QString getName() const;
    void setName(const QString &value);
};

} // namespace BatchProcess

#endif // BATCHPROCESS_IMAGESIGNAL_H
