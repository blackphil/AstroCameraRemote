#include "TimeUnitButton.h"



TimeUnitButton::TimeUnitButton(QWidget *parent)
    : QPushButton(parent)
    , connectedSpinBox(nullptr)
    , unit(Unit_Minutes)
{
    unitSettings[Unit_Minutes].name = tr( "min");
    unitSettings[Unit_Minutes].decimals = 3;
    unitSettings[Unit_Minutes].conversionFactor = 1. / 60000.;
    unitSettings[Unit_Minutes].convertToMSec = 60000.;
    unitSettings[Unit_Minutes].maxValue = 60;

    unitSettings[Unit_Seconds].name = tr(" sec");
    unitSettings[Unit_Seconds].decimals = 3;
    unitSettings[Unit_Seconds].conversionFactor = 60.;
    unitSettings[Unit_Seconds].convertToMSec = 1000.;
    unitSettings[Unit_Seconds].maxValue = 60 * 60;

    unitSettings[Unit_Milliseconds].name = tr(" msec");
    unitSettings[Unit_Milliseconds].decimals = 0;
    unitSettings[Unit_Milliseconds].conversionFactor = 1000.;
    unitSettings[Unit_Milliseconds].convertToMSec = 1.;
    unitSettings[Unit_Milliseconds].maxValue = 60 * 60 * 1000;

}

void TimeUnitButton::internalUpdate(bool updateSpinBoxValue)
{

    setText(unitSettings[unit].name);
    if(connectedSpinBox)
    {
        double oldValue = connectedSpinBox->value();
        connectedSpinBox->setMaximum(unitSettings[unit].maxValue);
        connectedSpinBox->setSuffix(unitSettings[unit].name);
        connectedSpinBox->setDecimals(unitSettings[unit].decimals);
        if(updateSpinBoxValue)
        {
            double newValue = oldValue * unitSettings[unit].conversionFactor;
            connectedSpinBox->setValue(newValue);
        }
    }
}


int TimeUnitButton::currentUnit() const
{
    return unit;
}

void TimeUnitButton::setCurrentUnit(int unit)
{
    this->unit = unit;
    internalUpdate(false);
}

int TimeUnitButton::getValueInMilliseconds() const
{
    if(connectedSpinBox)
        return qRound(connectedSpinBox->value() * unitSettings[unit].convertToMSec);

    Q_ASSERT(false && "no spin box connected!");
    return -1;
}

void TimeUnitButton::setValueInMilliseconds(int v)
{
    if(connectedSpinBox)
    {
        connectedSpinBox->setValue(v / unitSettings[unit].convertToMSec);
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
        connectedSpinBox->setSuffix(unitSettings[unit].name);
        connect(connectedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleValueChanged(double)));
    }
}

void TimeUnitButton::nextCheckState()
{
    int nextUnit = (unit + 1) % NumUnits;
    unit = static_cast<Unit>(nextUnit);
    internalUpdate(true);
    Q_EMIT unitChanged(unit);
}

void TimeUnitButton::handleValueChanged(double)
{
    int ms = getValueInMilliseconds();
    Q_EMIT valueChanged(ms);
}


