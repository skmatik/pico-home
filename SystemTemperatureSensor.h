#ifndef SYSTEMTEMPERATURESENSOR_H
#define SYSTEMTEMPERATURESENSOR_H

#include <string>
#include <memory>

#include "Sensor.h"

/*
 * @brief SystemTemperatureSensor class for reading build in temperature sensor on ADC4.
 */
class SystemTemperatureSensor : public Sensor
{
private:
    const static std::string UNITS_OF_MEASSUREMENT;
    const static std::string NAME;
    constexpr static int ADC_CHANNEL = 4;

    double temperature;

public:
    SystemTemperatureSensor();
    void read() override;
};

#endif // SYSTEMTEMPERATURESENSOR_H