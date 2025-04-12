#include <iostream>
#include "SensorMenuItem.h"


SensorMenuItem::SensorMenuItem(std::shared_ptr<Sensor> sensor) : sensor(sensor)
{
}

std::string_view SensorMenuItem::getItemText()
{
    unsigned int textSize = sensor->getSensorName().size() + 1 + sensor->getFormattedValue().size();
    itemText.clear();
    itemText.append(sensor->getSensorName())
        .append(":")
        .append(20 - textSize, ' ')
        .append(sensor->getFormattedValue());
    return itemText;
}
