#ifndef BATCHPROCESS_IMAGESIGNAL_H
#define BATCHPROCESS_IMAGESIGNAL_H

#include "batchprocess_global.h"

#include "BatchProcess_Signal.h"

#include "Fits_File.h"

#include <QObject>
#include <QList>
#include <QSharedPointer>

namespace BatchProcess {

class ImageSignal;
typedef QSharedPointer<ImageSignal> ImageSignalPtr;

class BATCHPROCESSSHARED_EXPORT ImageSignal : public Signal
{
    Q_DECLARE_TR_FUNCTIONS(BatchProcess::ImageSignal)

    ImageSignal(const ImageSignal& rhs, bool deep = false);
    bool operator=(const ImageSignal& rhs);

    QStringList files;
    QList<Fits::FilePtr> images;

public:
    ImageSignal(Direction direction, const QString& name);
    virtual ~ImageSignal();

    virtual ImageSignalPtr clone() const;
    int numImages() const;
    const QList<Fits::FilePtr>& getImages() const;
    void setImages(const QList<Fits::FilePtr> &value);

    double getPixel(int imageIndex, int pixelIndex) const;
    void setPixel(int imageIndex, int pixelIndex, const double& value);

    QString getTitle() const;

    void connectToSignal(SignalPtr other);

    bool edit();
};

} // namespace BatchProcess

#endif // BATCHPROCESS_IMAGESIGNAL_H
