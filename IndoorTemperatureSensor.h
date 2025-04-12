#ifndef INDOORTEMPERATURESENSOR_H
#define INDOORTEMPERATURESENSOR_H

#include <string>
#include <memory>
#include "Sensor.h"
#include "BMP280I2C.h"

class IndoorTemperatureSensor : public Sensor
{
private:
    const static std::string UNITS_OF_MEASSUREMENT;
    const static std::string NAME;

    BMP280I2C bmp280I2c;
    double temperature;

public:
    IndoorTemperatureSensor(BMP280I2C &bmp280I2c);
    void read() override;
};

#endif // INDOORTEMPERATURESENSOR_H