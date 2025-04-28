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
}

void WaterPressureSensor::read()
{
    // read from sensor
    // store in formattedValue
    adc_select_input(ADC_CHANNEL);
    
    uint16_t rawValue;
    float sum = 0;
    int count = 0;

    for (int i = 0; i < 10; i++)
    {
        rawValue = adc_read();
        if (rawValue > 0)
        {
            sum += rawValue;
            count++;
        }
        
    }

    if (count == 0)
    {
        setPresent(false);
        return;
    }
    
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = (sum / count)  * conversion_factor;
    pressure = ((voltage - 0.4852f) / 0.0046f);
    //pressure = voltage;
    setFormattedValue(formatDoubleValue(pressure));
    setPresent(true);
}

const std::string WaterPressureSensor::UNITS_OF_MEASSUREMENT = "kPa";
const std::string WaterPressureSensor::NAME = "Tlak vody";

