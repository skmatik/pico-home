#ifndef INDOORTEMPERATURESENSOR_H
#define INDOORTEMPERATURESENSOR_H

#include <string>
#include <memory>
#include "Sensor.h"
#include "BMP280I2C.h"

class IndoorTemperatureSensor : public Sensor
{
private:
    constexpr static std::string_view UNITS_OF_MEASSUREMENT = "C";
    constexpr static std::string_view NAME = "Kotolna";

    BMP280I2C bmp280I2c;
    bool present;
    double temperature;
    std::string formattedValue;

    std::string formatValue();
public:
    IndoorTemperatureSensor(BMP280I2C &bmp280I2c);
    std::string_view getName() override;
    std::string_view getUnitsOfMeassurement() override;
    std::string &getFormattedValue() override;
    void read() override;
    bool isPresent() override;
};

#endif // INDOORTEMPERATURESENSOR_H