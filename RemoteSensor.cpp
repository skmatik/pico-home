
#include "RemoteSensor.h"

#include <utility>
#include <sstream>

RemoteSensor::RemoteSensor(const std::string &name, const std::string &unitsOfMeasurement)
    : Sensor(name, unitsOfMeasurement), temperature(0.0)
{
}

void RemoteSensor::updateTemperatureString(const std::string &newValue)
{
    std::istringstream is(newValue.substr(2));
    is >> temperature;
    setFormattedValue(formatDoubleValue(temperature));
    setPresent(true);
}

void RemoteSensor::read()
{
    // nothing to do
}
