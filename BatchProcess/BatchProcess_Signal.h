#ifndef BATCHPROCESS_SIGNAL_H
#define BATCHPROCESS_SIGNAL_H

#include <QString>
#include <QSharedPointer>

class QWidget;

namespace BatchProcess {

class Signal;
typedef QSharedPointer<Signal> SignalPtr;

class Signal
{
public :

    enum Direction
    {
        Direction_In = 0x01
        , Direction_Out = 0x02
        , Direction_InOut = 0x04
    };

private :
    Direction direction;
    QString name;

    SignalPtr connectedTo;

protected :
    Signal(Direction direction, const QString& name = "") : direction(direction), name(name) {}
    Signal(const Signal& rhs) : direction(rhs.direction), name(rhs.name) {}

    void setConnectedTo(const SignalPtr &value);
public:
    virtual ~Signal() {}

    virtual double getPixel(int imageIndex, int pixelIndex) const = 0;
    virtual void setPixel(int imageIndex, int pixelIndex, const double& value) = 0;

    void setName(const QString& value) { name = value; }
    QString getName() const { return name; }

    virtual QString getTitle() const { return getName(); }

    virtual bool edit() { return false; }
    Direction getDirection() const;

    virtual void connectToSignal(SignalPtr other) { Q_UNUSED(other); }
    SignalPtr getConnectedTo() const;
};

typedef QSharedPointer<Signal> SignalPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_SIGNAL_H
