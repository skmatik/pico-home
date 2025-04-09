#ifndef ATMOSPHERICPRESSURESENSOR_H
#define ATMOSPHERICPRESSURESENSOR_H

#include <string>
#include <memory>

#include "Sensor.h"
#include "BMP280I2C.h"

class AtmosphericPressureSensor : public Sensor
{
private:
    constexpr static std::string_view UNITS_OF_MEASSUREMENT = "kPa";
    constexpr static std::string_view NAME = "Tlak vzd";

    BMP280I2C bmp280I2c;
    bool present;
    double pressure;
    std::string formattedValue;

    std::string formatValue();

public:
    AtmosphericPressureSensor(BMP280I2C &bmp280I2c);
    std::string_view getName() override;
    std::string_view getUnitsOfMeassurement() override;
    std::string &getFormattedValue() override;
    void read() override;
    bool isPresent() override;
};

#endif // ATMOSPHERICPRESSURESENSOR_H