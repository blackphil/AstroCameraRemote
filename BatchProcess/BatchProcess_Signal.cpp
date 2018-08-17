#include "BatchProcess_Signal.h"

namespace BatchProcess {

SignalPtr Signal::getConnectedTo() const
{
    return connectedTo;
}

void Signal::setConnectedTo(const SignalPtr &value)
{
    connectedTo = value;
}

Signal::Direction Signal::getDirection() const
{
    return direction;
}



} // namespace BatchProcess
