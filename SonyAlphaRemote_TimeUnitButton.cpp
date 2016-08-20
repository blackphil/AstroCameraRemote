#include "SonyAlphaRemote_TimeUnitButton.h"

namespace SonyAlphaRemote {

TimeUnitButton::TimeUnitButton(QWidget *parent)
    : QPushButton(parent)
    , connectedSpinBox(NULL)
    , unit(Unit_Minutes)
{
    unitSettings[Unit_Minutes].name = tr("min");
    unitSettings[Unit_Minutes].decimals = 3;
    unitSettings[Unit_Minutes].conversionFactor = 1. / 60000.;
    unitSettings[Unit_Minutes].convertToMSec = 60000.;
    unitSettings[Unit_Minutes].maxValue = 60;

    unitSettings[Unit_Seconds].name = tr("sec");
    unitSettings[Unit_Seconds].decimals = 3;
    unitSettings[Unit_Seconds].conversionFactor = 60.;
    unitSettings[Unit_Seconds].convertToMSec = 1000.;
    unitSettings[Unit_Seconds].maxValue = 60 * 60;

    unitSettings[Unit_Milliseconds].name = tr("msec");
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


TimeUnitButton::Unit TimeUnitButton::currentUnit() const
{
    return unit;
}

void TimeUnitButton::setCurrentUnit(TimeUnitButton::Unit unit)
{
    this->unit = unit;
    internalUpdate(false);
}

int TimeUnitButton::getValueInMilliseconds() const
{
    if(connectedSpinBox)
        return connectedSpinBox->value() * unitSettings[unit].convertToMSec;

    Q_ASSERT(!"no spin box connected!");
    return -1;
}

void TimeUnitButton::setValueInMilliseconds(int v)
{
    if(connectedSpinBox)
    {
        connectedSpinBox->setValue((double)v / unitSettings[unit].convertToMSec);
        return;
    }

    Q_ASSERT(!"no spin box connected!");
}

void TimeUnitButton::connectToSpinbox(QDoubleSpinBox *spinBox)
{
    connectedSpinBox = spinBox;
}

void TimeUnitButton::nextCheckState()
{
    int nextUnit = (unit + 1) % NumUnits;
    unit = (Unit)nextUnit;
    internalUpdate(true);
    Q_EMIT unitChanged(unit);
}

} // namespace SonyAlphaRemote
