#ifndef BATCHPROCESS_NUMERICSIGNAL_H
#define BATCHPROCESS_NUMERICSIGNAL_H

#include "batchprocess_global.h"
#include "BatchProcess_Signal.h"

#include <QSharedPointer>

namespace BatchProcess {

class BATCHPROCESSSHARED_EXPORT NumericSignal : public Signal
{
    Q_OBJECT

    double value;
public:
    NumericSignal(Direction direction, const QString& name, const double& value, QObject *parent);

    void setPixel(int imageIndex, int pixelIndex, const double& value);
    double getPixel(int imageIndex, int pixelIndex) const;

    QString getTitle() const;

    bool edit();
};

typedef QSharedPointer<NumericSignal> NumericSignalPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_NUMERICSIGNAL_H
