#ifndef HOMEPI_MQTTFEED_H
#define HOMEPI_MQTTFEED_H

#include <string>
#include "Sensor.h"

class MQTTFeed
{
private:
    std::string_view feedName;
    Sensor &sensor;

public:
    std::string &getFeedValue();
    std::string_view getFeedName();
    bool isPresent();
    MQTTFeed(const std::string_view& feedName, Sensor &sensor);
};

#endif // HOMEPI_MQTTFEED_H
