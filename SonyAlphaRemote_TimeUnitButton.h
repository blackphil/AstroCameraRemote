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

    Unit unit;
    void internalUpdate(bool updateSpinBoxValue);

Q_SIGNALS :
    void unitChanged(Unit unit);


public:
    TimeUnitButton(QWidget *parent);

    Unit currentUnit() const;
    void setCurrentUnit(Unit unit);

    int getValueInMilliseconds() const;

    void connectToSpinbox(QDoubleSpinBox *spinBox);

protected :
    void nextCheckState();
};

} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_TIMEUNITBUTTON_H
