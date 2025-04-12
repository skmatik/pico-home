#include "IndoorTemperatureSensor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

IndoorTemperatureSensor::IndoorTemperatureSensor(BMP280I2C &bmp280I2c) : Sensor(NAME, UNITS_OF_MEASSUREMENT)
{
    this->bmp280I2c = bmp280I2c;
    temperature = 0.0;
}

void IndoorTemperatureSensor::read()
{
    // read from sensor
    // store in formattedValue
    temperature = bmp280I2c.getTemperatureInDegC();
    setFormattedValue(formatDoubleValue(temperature));
    setPresent(true);
}

const std::string IndoorTemperatureSensor::UNITS_OF_MEASSUREMENT = "C";
const std::string IndoorTemperatureSensor::NAME = "Kotolna";



