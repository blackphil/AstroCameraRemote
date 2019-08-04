#include "TimeUnitButton.h"



TimeUnitButton::TimeUnitButton(QWidget *parent)
    : QPushButton(parent)
    , connectedSpinBox(nullptr)
    , unit(TimeUnit::Unit::Minutes)
{
}

void TimeUnitButton::internalUpdate(bool updateSpinBoxValue)
{

    setText(TimeUnits[static_cast<int>(unit)].name);
    if(connectedSpinBox)
    {
        double oldValue = connectedSpinBox->value();
        connectedSpinBox->setMaximum(TimeUnits[static_cast<int>(unit)].maxValue);
        connectedSpinBox->setSuffix(QString(" %0").arg(TimeUnits[static_cast<int>(unit)].name));
        connectedSpinBox->setDecimals(TimeUnits[static_cast<int>(unit)].decimals);
        if(updateSpinBoxValue)
        {
            double newValue = oldValue * TimeUnits[static_cast<int>(unit)].conversionFactor;
            connectedSpinBox->setValue(newValue);
        }
    }
}


TimeUnit::Unit TimeUnitButton::currentUnit() const
{
    return unit;
}

void TimeUnitButton::setCurrentUnit(TimeUnit::Unit unit)
{
    this->unit = unit;
    internalUpdate(false);
}

int TimeUnitButton::getValueInMilliseconds() const
{
    if(connectedSpinBox)
        return qRound(connectedSpinBox->value() * TimeUnits[static_cast<int>(unit)].convertToMSec);

    Q_ASSERT(false && "no spin box connected!");
    return -1;
}

void TimeUnitButton::setValueInMilliseconds(int v)
{
    if(connectedSpinBox)
    {
        connectedSpinBox->setValue(v / TimeUnits[static_cast<int>(unit)].convertToMSec);
        return;
    }

    Q_ASSERT(false && "no spin box connected!");
}

void TimeUnitButton::connectToSpinbox(QDoubleSpinBox *spinBox)
{
    if(connectedSpinBox)
        disconnect(spinBox, nullptr, this, nullptr);

    connectedSpinBox = spinBox;

    if(connectedSpinBox)
    {
        connectedSpinBox->setSuffix(QString(" %0").arg(TimeUnits[static_cast<int>(unit)].name));
        connect(connectedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleValueChanged(double)));
    }
}

void TimeUnitButton::nextCheckState()
{
    unit = static_cast<TimeUnit::Unit>((static_cast<uint>(unit) + 1) % NumTimeUnits);
    internalUpdate(true);
    Q_EMIT unitChanged(unit);
}

void TimeUnitButton::handleValueChanged(double)
{
    int ms = getValueInMilliseconds();
    Q_EMIT valueChanged(ms);
}


