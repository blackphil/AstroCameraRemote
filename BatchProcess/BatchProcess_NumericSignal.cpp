#include "BatchProcess_NumericSignal.h"

#include "BatchProcess_Manager.h"

#include <AstroBase/AstroBase>

#include <cmath>

namespace BatchProcess {

NumericSignal::NumericSignal(Direction direction, const QString &name, const double &value, QObject* parent)
    : Signal(direction, name, parent)
    , value(value)
{

}

void NumericSignal::setPixel(int imageIndex, int pixelIndex, const double &value)
{
    Q_UNUSED(imageIndex)
    Q_UNUSED(pixelIndex)

    this->value = value;
}

double NumericSignal::getPixel(int imageIndex, int pixelIndex) const
{
    Q_UNUSED(imageIndex)
    Q_UNUSED(pixelIndex)

    return value;
}

QString NumericSignal::getTitle() const
{
    return QString("%0: %1").arg(getName()).arg(value);
}

bool NumericSignal::edit()
{
    double newVal = Manager::get()->getNumericValueGui(value);
    if(!std::isnan(newVal))
    {
        value = newVal;
        Signal::edit();
        return true;
    }

    return false;
}

} // namespace BatchProcess
