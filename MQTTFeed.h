#ifndef MQTTFEED_H
#define MQTTFEED_H

#include <string>
#include <memory>
#include "Sensor.h"

class MQTTFeed
{
private:
    std::string feedName;
    std::shared_ptr<Sensor> sensor;

public:
    const std::string &getFeedValue();
    const std::string &getFeedName();
    bool isPresent();
    MQTTFeed(const std::string& feedName, std::shared_ptr<Sensor> sensor);
};

#endif // MQTTFEED_H
