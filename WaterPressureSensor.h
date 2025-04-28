#ifndef WATERPRESSURESENSOR_H
#define WATERPRESSURESENSOR_H

#include <string>
#include <memory>

#include "Sensor.h"

/*
 * @brief WaterPressureSensor class for reading water pressure sensor values from 1/8NPT.
 * This sensor is converting water pressure to voltage using a piezoresistive sensor. 5V represents 100PSI.
 * The sensor is connected to the ADC channel 0 and GPIO pin 26.
 */
class WaterPressureSensor : public Sensor
{
private:
    const static std::string UNITS_OF_MEASSUREMENT;
    const static std::string NAME;
    constexpr static int ADC_CHANNEL = 0; // ADC channel for the water pressure sensor
    constexpr static int ADC_GPIO = 26;   // GPIO pin for the water pressure sensor

    double pressure;
public:
    WaterPressureSensor();
    void read() override;
};

#endif // WATERPRESSURESENSOR_H