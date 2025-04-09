#ifndef WATERPRESSURESENSOR_H
#define WATERPRESSURESENSOR_H

#include <string>
#include <memory>

#include "Sensor.h"

class WaterPressureSensor : public Sensor
{
private:
    constexpr static std::string_view UNITS_OF_MEASSUREMENT = "bar";
    constexpr static std::string_view NAME = "Tlak vody";
    constexpr static int ADC_CHANNEL = 0; // ADC channel for the water pressure sensor
    constexpr static int ADC_GPIO = 26; // GPIO pin for the water pressure sensor
    bool present;
    double pressure;
    std::string formattedValue;

    std::string formatValue();

public:
WaterPressureSensor();
    std::string_view getName() override;
    std::string_view getUnitsOfMeassurement() override;
    std::string &getFormattedValue() override;
    void read() override;
    bool isPresent() override;
};

#endif // WATERPRESSURESENSOR_H