#ifndef REMOTESENSOR_H
#define REMOTESENSOR_H

#include "Sensor.h"

class RemoteSensor : public Sensor {
private:
    double temperature;
public:
    RemoteSensor(const std::string &name, const std::string &unitsOfMeasurement);
    void read() override;

    void updateTemperatureString(const std::string & newValue);
};


#endif //REMOTESENSOR_H
