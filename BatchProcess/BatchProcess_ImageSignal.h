#ifndef BATCHPROCESS_IMAGESIGNAL_H
#define BATCHPROCESS_IMAGESIGNAL_H

#include "batchprocess_global.h"

#include "BatchProcess_Signal.h"

#include "Fits_File.h"

#include <QObject>
#include <QList>
#include <QPointer>

namespace BatchProcess {

class ImageSignal;
typedef QPointer<ImageSignal> ImageSignalPtr;

class BATCHPROCESSSHARED_EXPORT ImageSignal : public Signal
{
    Q_OBJECT

    ImageSignal(const ImageSignal& rhs) : Signal(rhs) {}
    bool operator=(const ImageSignal&) { return false; }

    QList<Fits::FilePtr> images;

public Q_SLOTS :
    void connectedSignalChanged();

public:
    explicit ImageSignal(Direction direction, const QString& name, QObject *parent);
    virtual ~ImageSignal();

    virtual ImageSignalPtr clone() const;
    int numImages() const;
    int numPixels() const;

    const QList<Fits::FilePtr>& getImages() const;
    void setImages(const QList<Fits::FilePtr> &value);

    double getPixel(int imageIndex, int pixelIndex) const;
    void setPixel(int imageIndex, int pixelIndex, const double& value);

    QString getTitle() const;

    void connectToSignal(Signal* other);

    bool edit();
};
typedef QPointer<ImageSignal> ImageSignalPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_IMAGESIGNAL_H
