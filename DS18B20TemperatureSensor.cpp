#include "DS18B20TemperatureSensor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

DS18B20TemperatureSensor::DS18B20TemperatureSensor(DS18B20 &ds18b20, const std::string &name, uint64_t romCode) : Sensor(name, UNITS_OF_MEASSUREMENT), ds18b20(ds18b20), romCode(romCode)
{
    temperature = 0.0;
    setPresent(false);
    setFormattedValue(NA);
}

void DS18B20TemperatureSensor::read()
{
    // read from sensor
    // store in formattedValue
    ds18b20.readTemperatures();
    if (ds18b20.isPresent(romCode))
    {
        temperature = ds18b20.getTemperature(romCode) / 16.0;
        setFormattedValue(formatDoubleValue(temperature));
        setPresent(true);
    } else {
        temperature = 0.0;
        setPresent(false);
        setFormattedValue(NA);    
    }
}

const std::string DS18B20TemperatureSensor::UNITS_OF_MEASSUREMENT = "C";


