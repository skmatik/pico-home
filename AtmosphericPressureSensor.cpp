#include "AtmosphericPressureSensor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

AtmosphericPressureSensor::AtmosphericPressureSensor(BMP280I2C &bmp280I2c) : bmp280I2c(bmp280I2c)
{
    pressure = 0.0;
    formattedValue = "0.0";
}

std::string_view AtmosphericPressureSensor::getName()
{
    return NAME;
}

std::string &AtmosphericPressureSensor::getFormattedValue()
{
    return formattedValue;
}

std::string_view AtmosphericPressureSensor::getUnitsOfMeassurement()
{
    return AtmosphericPressureSensor::UNITS_OF_MEASSUREMENT;
}

void AtmosphericPressureSensor::read()
{
    // read from sensor
    // store in formattedValue
    pressure = bmp280I2c.getPressureInkPa();
    formattedValue = formatValue();
    present = true;
}

bool AtmosphericPressureSensor::isPresent()
{
    return present;
}

std::string AtmosphericPressureSensor::formatValue() {
    std::ostringstream formattedValueStream;
    formattedValueStream << std::setprecision(2);
    formattedValueStream << std::fixed;
    formattedValueStream << pressure;
    return formattedValueStream.str();
}
