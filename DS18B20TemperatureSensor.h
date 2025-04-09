#ifndef DS18B20TEMPERATURESENSOR_H
#define DS18B20TEMPERATURESENSOR_H

#include <string>
#include <memory>
#include "Sensor.h"
#include "DS18B20.h"

class DS18B20TemperatureSensor : public Sensor
{
private:
    constexpr static std::string_view UNITS_OF_MEASSUREMENT = "C";
    DS18B20 ds18b20;
    bool present;
    double temperature;
    std::string formattedValue;
    std::string_view name;
    uint64_t romCode;

    std::string formatValue();
public:
DS18B20TemperatureSensor(DS18B20 &db18b20, const std::string_view &name, uint64_t romCode);
    std::string_view getName() override;
    std::string_view getUnitsOfMeassurement() override;
    std::string &getFormattedValue() override;
    void read() override;
    bool isPresent() override;
};

#endif // DS18B20TEMPERATURESENSOR_H