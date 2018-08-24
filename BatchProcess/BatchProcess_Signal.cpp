#include "BatchProcess_Signal.h"

namespace BatchProcess {

Signal *Signal::getConnectedTo() const
{
    return connectedTo;
}

void Signal::setConnectedTo(Signal *&value)
{
    connectedTo = value;
    Q_EMIT signalChanged();
}

void Signal::setName(const QString &value)
{
    name = value;
    Q_EMIT signalChanged();
}

bool Signal::edit()
{
    Q_EMIT signalChanged();
    return true;
}

Signal::Direction Signal::getDirection() const
{
    return direction;
}

void Signal::connectToSignal(Signal *other)
{
    Q_UNUSED(other)
    Q_EMIT signalChanged();
}



} // namespace BatchProcess
