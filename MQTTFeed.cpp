#include "MQTTFeed.h"

MQTTFeed::MQTTFeed(const std::string_view& feedName, Sensor &sensor) : feedName(feedName), sensor(sensor)
{
    sensor.read();
}

std::string_view MQTTFeed::getFeedName()
{
    return feedName;
}

std::string &MQTTFeed::getFeedValue()
{
    return sensor.getFormattedValue();
}

bool MQTTFeed::isPresent()
{
    return sensor.isPresent();
}