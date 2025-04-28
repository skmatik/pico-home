#include "SystemTemperatureSensor.h"
#include <iomanip>
#include <sstream>
#include "hardware/adc.h"
#include <iostream>

SystemTemperatureSensor::SystemTemperatureSensor()
    : Sensor(NAME, UNITS_OF_MEASSUREMENT)
{
    adc_init();
    adc_set_temp_sensor_enabled(true);
}

void SystemTemperatureSensor::read()
{
    // read from sensor
    // store in formattedValue
    const float conversionFactor = 3.3f / (1 << 12);
    adc_select_input(ADC_CHANNEL);
    float rawValue = (float) adc_read() * conversionFactor;
    temperature = 27.0f - (rawValue - 0.706f) / 0.001721f;
    setFormattedValue(formatDoubleValue(temperature));
    setPresent(true);
}


const std::string SystemTemperatureSensor::UNITS_OF_MEASSUREMENT = "C";
const std::string SystemTemperatureSensor::NAME = "Teplota systemu";

