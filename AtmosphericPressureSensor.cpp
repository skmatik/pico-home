#include "AtmosphericPressureSensor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

AtmosphericPressureSensor::AtmosphericPressureSensor(BMP280I2C &bmp280I2c) : Sensor(NAME, UNITS_OF_MEASSUREMENT), bmp280I2c(bmp280I2c)
{
    pressure = 0.0;
}

void AtmosphericPressureSensor::read()
{
    // read from sensor
    // store in formattedValue
    pressure = bmp280I2c.getPressureInkPa();
    setFormattedValue(formatDoubleValue(pressure));
    setPresent(true);
}

const std::string AtmosphericPressureSensor::UNITS_OF_MEASSUREMENT = "kPa";
const std::string AtmosphericPressureSensor::NAME = "Tlak vzd";


