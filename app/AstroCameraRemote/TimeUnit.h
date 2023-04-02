#ifndef TIMEUNIT_H
#define TIMEUNIT_H


struct TimeUnit
{
public :
    enum class Unit
    {
        Minutes
        , Seconds
        , Milliseconds
    };


    const char* name;
    int decimals;
    double conversionFactor;
    double convertToMSec;
    double maxValue;


};

static constexpr const TimeUnit TimeUnits[]
{
    { "min", 3, 1. / 60000., 60000., 60 }
    , { "sec", 3, 60., 1000., 60 * 60 }
    , { "msec", 0, 1000., 1., 60 * 60 * 1000 }
};

static constexpr const auto NumTimeUnits { sizeof(TimeUnits) / sizeof(TimeUnit) };
#endif // TIMEUNIT_H
