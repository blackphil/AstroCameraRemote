#ifndef BATCHPROCESS_SIGNAL_H
#define BATCHPROCESS_SIGNAL_H

#include <QObject>
#include <QString>
#include <QSharedPointer>

class QWidget;

namespace BatchProcess {

class Signal : public QObject
{
    Q_OBJECT

public :

    enum Direction
    {
        Direction_In = 0x01
        , Direction_Out = 0x02
        , Direction_InOut = 0x04
    };

Q_SIGNALS :
    void signalChanged();

public Q_SLOTS :
    virtual void connectedSignalChanged() {}

private :
    Direction direction;
    QString name;

    Signal* connectedTo;

protected :
    Signal(Direction direction, const QString& name, QObject* parent) : QObject(parent), direction(direction), name(name) {}
    Signal(const Signal&) {}

    void setConnectedTo(Signal *&value);
public:
    virtual ~Signal() {}

    virtual double getPixel(int imageIndex, int pixelIndex) const = 0;
    virtual void setPixel(int imageIndex, int pixelIndex, const double& value) = 0;

    void setName(const QString& value);
    QString getName() const { return name; }

    virtual QString getTitle() const { return getName(); }

    virtual bool edit();
    Direction getDirection() const;

    virtual void connectToSignal(Signal* other);
    Signal* getConnectedTo() const;
};

typedef QPointer<Signal> SignalPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_SIGNAL_H
