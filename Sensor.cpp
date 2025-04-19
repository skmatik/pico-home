#include "Sensor.h"
#include <iomanip>

Sensor::Sensor(const std::string &sensorName, const std::string &unitsOfMeasurement)
    : sensorName(sensorName), unitsOfMeasurement(unitsOfMeasurement), present(false), formattedValue("0.0")
{
}

const std::string& Sensor::getSensorName() const
{
    return sensorName;
}

const std::string &Sensor::getFormattedValue() const
{
    return formattedValue;
}

const std::string &Sensor::getUnitsOfMeasurement() const
{
    return unitsOfMeasurement;
}

bool Sensor::isPresent()
{
    return present;
}


void Sensor::setPresent(bool present)
{
    this->present = present;
}

void Sensor::setFormattedValue(const std::string &formattedValue)
{
    this->formattedValue = formattedValue;
}

std::string Sensor::formatDoubleValue(double value) {
    std::ostringstream formattedValueStream;
    formattedValueStream << std::setprecision(2);
    formattedValueStream << std::fixed;
    formattedValueStream << value;
    return formattedValueStream.str();
}

const std::string Sensor::NA = "N/A";
