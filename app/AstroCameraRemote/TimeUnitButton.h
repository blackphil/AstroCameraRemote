#ifndef TIMEUNITBUTTON_H
#define TIMEUNITBUTTON_H

#include <QPushButton>
#include <QDoubleSpinBox>

#include "TimeUnit.h"



class TimeUnitButton : public QPushButton
{
    Q_OBJECT

    QDoubleSpinBox* connectedSpinBox;

    TimeUnit::Unit unit;
    void internalUpdate(bool updateSpinBoxValue);

Q_SIGNALS :
    void unitChanged(TimeUnit::Unit);
    void valueChanged(int);


public:
    TimeUnitButton(QWidget *parent);

    TimeUnit::Unit currentUnit() const;
    void setCurrentUnit(TimeUnit::Unit unit);

    int getValueInMilliseconds() const;
    void setValueInMilliseconds(int v);

    void connectToSpinbox(QDoubleSpinBox *spinBox);

protected :
    void nextCheckState();

protected Q_SLOTS :
    void handleValueChanged(double);
};



#endif // TIMEUNITBUTTON_H
