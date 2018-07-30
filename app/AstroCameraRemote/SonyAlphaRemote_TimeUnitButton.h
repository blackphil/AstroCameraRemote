#ifndef SONYALPHAREMOTE_TIMEUNITBUTTON_H
#define SONYALPHAREMOTE_TIMEUNITBUTTON_H

#include <QPushButton>
#include <QDoubleSpinBox>


namespace SonyAlphaRemote {

class TimeUnitButton : public QPushButton
{
    Q_OBJECT

public :
    enum Unit
    {
        Unit_Minutes
        , Unit_Seconds
        , Unit_Milliseconds
        , NumUnits
    };



private :
    struct UnitSettings
    {
        QString name;
        int decimals;
        double conversionFactor;
        double convertToMSec;
        double maxValue;
    };
    UnitSettings unitSettings[NumUnits];


    QDoubleSpinBox* connectedSpinBox;

    int unit;
    void internalUpdate(bool updateSpinBoxValue);

Q_SIGNALS :
    void unitChanged(int);
    void valueChanged(int);


public:
    TimeUnitButton(QWidget *parent);

    int currentUnit() const;
    void setCurrentUnit(int unit);

    int getValueInMilliseconds() const;
    void setValueInMilliseconds(int v);

    void connectToSpinbox(QDoubleSpinBox *spinBox);

protected :
    void nextCheckState();

protected Q_SLOTS :
    void handleValueChanged(double);
};

} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_TIMEUNITBUTTON_H
