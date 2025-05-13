#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

#include <string>
#include <vector>
#include "FreeRTOS.h"
#include "task.h"

#include "TimeConstants.h"
#include "MQTTFeed.h"
#include "Sensor.h"
#include "mqtt_interface.h"
#include "EthernetController.h"
#include "MQTTClient.h"

constexpr uint SOCKET_MQTT = 0;
constexpr uint PORT_MQTT = 1883;

class MQTTPublisher
{

private:
    static constexpr int TASK_STACK_SIZE = configMINIMAL_STACK_SIZE;
    static constexpr int TASK_PRIORITY = tskIDLE_PRIORITY;
    static constexpr std::string_view MQTT_CLIENT_ID = "Pico Home";
    static constexpr std::string_view MQTT_USERNAME = "skmatik";
    static constexpr std::string_view FEED_PREFIX = "skmatik/feeds/";
    static constexpr int MQTT_KEEP_ALIVE = 60;
    static constexpr int MQTT_PUBLISH_PERIOD = 60 * 1000; // 60 seconds
    static constexpr int QOS = 1;
    static constexpr long TIMEOUT = 10000L;
    static constexpr long INTERVAL_IN_SECONDS = 10;
    static const int KEEP_ALIVE_IN_SECONDS = 20;
    static uint8_t sendBuffer[ETHERNET_BUFFER_SIZE];
    static uint8_t receiveBuffer[ETHERNET_BUFFER_SIZE];
    static constexpr int DEFAULT_TIMEOUT = 1000; // 1 second

    Network mqttNetwork;
    MQTTClient mqttClient;
    std::shared_ptr<std::vector<std::unique_ptr<MQTTFeed>>> feeds;
    EthernetController ethernetController;
    TaskHandle_t task;
    bool mqttConnected;

    static void publishTask(__unused void *params);
    void logResult(uint8_t retval, const std::string &message);

    static void createMessage(const std::string &value, MQTTMessage &mqttMessage);

    void publishOneMessage(const std::string &feedTopic, MQTTMessage &g_mqtt_message);
    void publish();
    void connect();
    void disconnect();
public:
    MQTTPublisher(std::shared_ptr<std::vector<std::unique_ptr<MQTTFeed>>> feeds, EthernetController &ethernetController);
    virtual ~MQTTPublisher();
    bool isConnected();
    void publishMessage(const std::string &feedTopic, const std::string &value);
};

#endif // MQTTPUBLISHER_H
