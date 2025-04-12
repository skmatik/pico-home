#ifndef HOMEPI_MQTTFEED_H
#define HOMEPI_MQTTFEED_H

#include <string>
#include "Sensor.h"

class MQTTFeed
{
private:
    std::string feedName;
    Sensor &sensor;

public:
    const std::string &getFeedValue();
    const std::string &getFeedName();
    bool isPresent();
    MQTTFeed(const std::string& feedName, Sensor &sensor);
};

#endif // HOMEPI_MQTTFEED_H
