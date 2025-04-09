#include "DS18B20TemperatureSensor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

DS18B20TemperatureSensor::DS18B20TemperatureSensor(DS18B20 &ds18b20, const std::string_view &name, uint64_t romCode) : ds18b20(ds18b20), name(name), romCode(romCode)
{
    present = false;
    temperature = 0.0;
    formattedValue = "0.0";
}

std::string_view DS18B20TemperatureSensor::getName()
{
    return name;
}

std::string &DS18B20TemperatureSensor::getFormattedValue()
{
    return formattedValue;
}

std::string_view DS18B20TemperatureSensor::getUnitsOfMeassurement()
{
    return UNITS_OF_MEASSUREMENT;
}

void DS18B20TemperatureSensor::read()
{
    // read from sensor
    // store in formattedValue
    ds18b20.readTemperatures();
    temperature = ds18b20.getTemperature(romCode) / 16.0;
    formattedValue = formatValue();
    present = true;
}

bool DS18B20TemperatureSensor::isPresent()
{
    return present;
}

std::string DS18B20TemperatureSensor::formatValue() {
    std::ostringstream formattedValueStream;
    formattedValueStream << std::setprecision(2);
    formattedValueStream << std::fixed;
    formattedValueStream << temperature;
    return formattedValueStream.str();
}


