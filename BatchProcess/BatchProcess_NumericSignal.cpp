#include "BatchProcess_NumericSignal.h"

#include "BatchProcess_Manager.h"

namespace BatchProcess {

NumericSignal::NumericSignal(const QString &name, const double &value)
    : Signal(name)
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
    if(std::numeric_limits<double>::signaling_NaN() != newVal)
    {
        value = newVal;
        return true;
    }

    return false;
}

} // namespace BatchProcess
