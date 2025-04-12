#ifndef ATMOSPHERICPRESSURESENSOR_H
#define ATMOSPHERICPRESSURESENSOR_H

#include <string>
#include <memory>

#include "Sensor.h"
#include "BMP280I2C.h"

class AtmosphericPressureSensor : public Sensor
{
private:
    const static std::string UNITS_OF_MEASSUREMENT;
    const static std::string NAME;

    BMP280I2C bmp280I2c;
    double pressure;

public:
    AtmosphericPressureSensor(BMP280I2C &bmp280I2c);
    void read() override;
};

#endif // ATMOSPHERICPRESSURESENSOR_H