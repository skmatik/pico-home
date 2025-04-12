#include "WaterPressureSensor.h"
#include <iomanip>
#include <sstream>
#include "hardware/adc.h"
#include <iostream>

WaterPressureSensor::WaterPressureSensor()
    : Sensor(NAME, UNITS_OF_MEASSUREMENT)
{
    adc_init();
    adc_gpio_init(ADC_GPIO);
    adc_select_input(ADC_CHANNEL);
}

void WaterPressureSensor::read()
{
    // read from sensor
    // store in formattedValue
    uint16_t rawValue = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = rawValue * conversion_factor;
    pressure = 0.01 * ((voltage - 0.4852f) / 0.0046f);   
    setFormattedValue(formatDoubleValue(pressure));
    setPresent(true);
}

const std::string WaterPressureSensor::UNITS_OF_MEASSUREMENT = "bar";
const std::string WaterPressureSensor::NAME = "Tlak vody";

