#include "WaterPressureSensor.h"
#include <iomanip>
#include <sstream>
#include "hardware/adc.h"
#include <iostream>

WaterPressureSensor::WaterPressureSensor()
{
    adc_init();
    adc_gpio_init(ADC_GPIO);
    adc_select_input(ADC_CHANNEL);
    formattedValue = "0.0";
    present = false;
}

std::string_view WaterPressureSensor::getName()
{
    return NAME;
}

std::string &WaterPressureSensor::getFormattedValue()
{
    return formattedValue;
}

std::string_view WaterPressureSensor::getUnitsOfMeassurement()
{
    return UNITS_OF_MEASSUREMENT;
}

void WaterPressureSensor::read()
{
    // read from sensor
    // store in formattedValue
    uint16_t rawValue = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = rawValue * conversion_factor;
    pressure = 0.01 * ((voltage - 0.4852f) / 0.0046f);   
    formattedValue = formatValue();
    present = true;
}

bool WaterPressureSensor::isPresent()
{
    return present;
}

std::string WaterPressureSensor::formatValue() {
    std::ostringstream formattedValueStream;
    formattedValueStream << std::setprecision(2);
    formattedValueStream << std::fixed;
    formattedValueStream << pressure;
    return formattedValueStream.str();
}
