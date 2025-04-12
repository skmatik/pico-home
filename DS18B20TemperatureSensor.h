#ifndef DS18B20TEMPERATURESENSOR_H
#define DS18B20TEMPERATURESENSOR_H

#include <string>
#include <memory>
#include "Sensor.h"
#include "DS18B20.h"

class DS18B20TemperatureSensor : public Sensor
{
private:
    const static std::string UNITS_OF_MEASSUREMENT;
    ;
    DS18B20 ds18b20;
    double temperature;
    uint64_t romCode;

    std::string formatValue();

public:
    DS18B20TemperatureSensor(DS18B20 &db18b20, const std::string &name, uint64_t romCode);
    void read() override;
};

#endif // DS18B20TEMPERATURESENSOR_H