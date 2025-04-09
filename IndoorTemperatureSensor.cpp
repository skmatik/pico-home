#include "IndoorTemperatureSensor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

IndoorTemperatureSensor::IndoorTemperatureSensor(BMP280I2C &bmp280I2c)
{
    this->bmp280I2c = bmp280I2c;
    temperature = 0.0;
    formattedValue = "0.0";
}

std::string_view IndoorTemperatureSensor::getName()
{
    return NAME;
}

std::string &IndoorTemperatureSensor::getFormattedValue()
{
    return formattedValue;
}

std::string_view IndoorTemperatureSensor::getUnitsOfMeassurement()
{
    return IndoorTemperatureSensor::UNITS_OF_MEASSUREMENT;
}

void IndoorTemperatureSensor::read()
{
    // read from sensor
    // store in formattedValue
    temperature = bmp280I2c.getTemperatureInDegC();
    formattedValue = formatValue();
    present = true;
}

bool IndoorTemperatureSensor::isPresent()
{
    return present;
}

std::string IndoorTemperatureSensor::formatValue() {
    std::ostringstream formattedValueStream;
    formattedValueStream << std::setprecision(2);
    formattedValueStream << std::fixed;
    formattedValueStream << temperature;
    return formattedValueStream.str();
}


