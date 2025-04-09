#ifndef SENSOR_H
#define SENSOR_H

#include <string>

class Sensor
{
public:
    virtual std::string_view getName() = 0;
    virtual std::string &getFormattedValue() = 0;
    virtual std::string_view getUnitsOfMeassurement() = 0;
    virtual void read() = 0;
    virtual bool isPresent() = 0;
};

#endif // SENSOR_H
