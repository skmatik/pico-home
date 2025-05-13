#include <cstring>
#include <iostream>
#include "MQTTPublisher.h"

MQTTPublisher::MQTTPublisher(std::shared_ptr<std::vector<std::unique_ptr<MQTTFeed>>> feeds, EthernetController &ethernetController)
    : feeds(feeds), ethernetController(ethernetController)
{
    mqttConnected = false;
    xTaskCreate(MQTTPublisher::publishTask, "MQTTPublisherThread", TASK_STACK_SIZE, (void *)this, TASK_PRIORITY, &task);
}

MQTTPublisher::~MQTTPublisher()
{
}

bool MQTTPublisher::isConnected()
{
    return mqttConnected;
}

void MQTTPublisher::publishMessage(const std::string &feedTopic, const std::string &value) {
    connect();
    MQTTMessage mqttMessage;
    createMessage(value, mqttMessage);
    std::string feedTopicFullName = string(FEED_PREFIX).append(feedTopic);
    publishOneMessage(feedTopicFullName, mqttMessage);
}

void MQTTPublisher::connect()
{
    NewNetwork(&mqttNetwork, SOCKET_MQTT);

    std::unique_ptr<uint8_t[]> mqttServerIp = ethernetController.resolveDomain((uint8_t *)"io.adafruit.com");

    uint8_t retval = ConnectNetwork(&mqttNetwork, mqttServerIp.get(), PORT_MQTT);

    if (retval != 1)
    {
        cout << "Network connection failed" << endl;
    }

    /* Initialize MQTT client */
    MQTTClientInit(&mqttClient, &mqttNetwork, DEFAULT_TIMEOUT, sendBuffer, ETHERNET_BUFFER_SIZE, receiveBuffer, ETHERNET_BUFFER_SIZE);

    /* Connect to the MQTT broker */
    MQTTPacket_connectData mqttConnectionPacket = MQTTPacket_connectData_initializer;
    mqttConnectionPacket.MQTTVersion = 3;
    mqttConnectionPacket.cleansession = 1;
    mqttConnectionPacket.willFlag = 0;
    mqttConnectionPacket.keepAliveInterval = MQTT_KEEP_ALIVE;
    mqttConnectionPacket.clientID.cstring = const_cast<char *>(MQTT_CLIENT_ID.data());
    mqttConnectionPacket.username.cstring = const_cast<char *>(MQTT_USERNAME.data());
    mqttConnectionPacket.password.cstring = const_cast<char *>(MQTT_PASSWORD);
    retval = MQTTConnect(&mqttClient, &mqttConnectionPacket);

    logResult(retval, "MQTT connection");
    mqttConnected = (retval >= 0);
}

void MQTTPublisher::logResult(uint8_t retval, const std::string &message)
{
    if (retval < 0)
    {
        cout << message << " failed : " << retval << endl;
    }
    else
    {
        //cout << message << " succeeded" << endl;
    }
}

void MQTTPublisher::createMessage(const std::string &feedValue, MQTTMessage &mqttMessage) {
    mqttMessage.qos = QOS0;
    mqttMessage.retained = 0;
    mqttMessage.dup = 0;
    const char* value = feedValue.c_str();
    mqttMessage.payload = (void *) value;
    mqttMessage.payloadlen = strlen(value);
}

void MQTTPublisher::publish()
{

        cout << "Publishing MQTT Feeds: " << feeds->size()  << endl;

        for (auto &feed : *feeds)
        {
            if (feed->isPresent())
            {
                //cout << "Publishing feed: " << feed->getFeedName() << endl;
                MQTTMessage mqttMessage;
                createMessage(feed->getFeedValue(), mqttMessage);

                string feedTopic = string(FEED_PREFIX).append(feed->getFeedName());
                publishOneMessage(feedTopic, mqttMessage);
            }
        }
}

void MQTTPublisher::publishOneMessage(const std::string &feedTopic, MQTTMessage &g_mqtt_message)
{
    //cout << "Publishing: " << (char*) g_mqtt_message.payload << " to feed: " << feedTopic << endl;
    uint8_t retval = MQTTPublish(&mqttClient, feedTopic.c_str(), &g_mqtt_message);
    logResult(retval, "MQTT publish");

    if ((retval = MQTTYield(&mqttClient, MQTT_KEEP_ALIVE)) < 0)
    {
        cout << "MQTT Yield error : " << retval << endl;
        while (1)
            ;
    }
}

void MQTTPublisher::disconnect()
{
    cout << "Disconnecting MQTT" << endl;
}


void MQTTPublisher::publishTask(__unused void *params)
{
    MQTTPublisher *mqttPublisher = (MQTTPublisher *)params;
    mqttPublisher->connect();
    while (true)
    {
        mqttPublisher->publish();
        vTaskDelay(pdMS_TO_TICKS(MQTT_PUBLISH_PERIOD));
    }
}



uint8_t MQTTPublisher::sendBuffer[ETHERNET_BUFFER_SIZE] = {0};
uint8_t MQTTPublisher::receiveBuffer[ETHERNET_BUFFER_SIZE] = {0};
