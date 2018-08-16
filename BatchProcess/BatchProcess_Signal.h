#ifndef BATCHPROCESS_SIGNAL_H
#define BATCHPROCESS_SIGNAL_H

#include <QString>
#include <QSharedPointer>

class QWidget;

namespace BatchProcess {

class Signal
{
    QString name;

public:
    Signal(const QString& name = "") : name(name) {}
    virtual ~Signal() {}

    virtual double getPixel(int imageIndex, int pixelIndex) const = 0;
    virtual void setPixel(int imageIndex, int pixelIndex, const double& value) = 0;

    void setName(const QString& value) { name = value; }
    QString getName() const { return name; }

    virtual QString getTitle() const { return getName(); }

    virtual bool edit() { return false; }
};

typedef QSharedPointer<Signal> SignalPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_SIGNAL_H
