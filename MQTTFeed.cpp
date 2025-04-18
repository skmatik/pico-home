#include "MQTTFeed.h"

MQTTFeed::MQTTFeed(const std::string& feedName, std::shared_ptr<Sensor> sensor) : feedName(feedName), sensor(sensor)
{
    sensor->read();
}

const std::string &MQTTFeed::getFeedName()
{
    return feedName;
}

const std::string &MQTTFeed::getFeedValue()
{
    return sensor->getFormattedValue();
}

bool MQTTFeed::isPresent()
{
    return sensor->isPresent();
}