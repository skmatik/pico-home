#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <vector>

#include "LcdDisplay.h"
#include "RotaryEncoder.h"
#include "EthernetController.h"
#include "MQTTPublisher.h"
#include "MQTTFeed.h"
#include "Bmp280I2C.h"
#include "AtmosphericPressureSensor.h"
#include "IndoorTemperatureSensor.h"
#include "WaterPressureSensor.h"
#include "LcdMenu.h"
#include "Sensor.h"
#include "MenuItem.h"
#include "SensorMenuItem.h"
#include "DS18B20.h"
#include "DS18B20TemperatureSensor.h"
#include "SerialCommunicator.h"

class MainController
{
private:
    static constexpr int MAIN_TASK_STACK_SIZE = configMINIMAL_STACK_SIZE;
    static constexpr int MAIN_TASK_PRIORITY = tskIDLE_PRIORITY;
    static constexpr int SENSOR_TASK_STACK_SIZE = configMINIMAL_STACK_SIZE;
    static constexpr int SENSOR_TASK_PRIORITY = tskIDLE_PRIORITY;

    // In C++, the order of field initialization is defined by the order in which fields are declared in the class, NOT the order in the constructor initializer list.
    LcdDisplay lcdDisplay;
    RotaryEncoder rotaryEncoder;
    EthernetController ethernetController;
    BMP280I2C bmp280I2c;
    DS18B20 ds18b20;
    std::shared_ptr<std::vector<std::unique_ptr<MQTTFeed>>> feeds;
    std::shared_ptr<std::vector<std::shared_ptr<MenuItem>>> menuItems;
    std::shared_ptr<std::vector<std::shared_ptr<Sensor>>> sensors;

    LcdMenu lcdMenu;
    AtmosphericPressureSensor atmosphericPressureSensor;
    IndoorTemperatureSensor indoorTemperatureSensor;
    WaterPressureSensor waterPressureSensor;
    DS18B20TemperatureSensor ds18b20TemperatureSensor;
    
    MQTTPublisher mqttPublisher;
    ArduinoTerminalReportBuilder arduinoTerminalReportBuilder;
    SerialCommunicator serialCommunicator;

    TaskHandle_t mainTask;
    TaskHandle_t sensorTask;

    static void updateSensors(__unused void *params);
    static void execute(__unused void *params);
    void loop();
    void initializeMenu();

public:
    MainController();
    ~MainController();
    void initialize();
};

#endif // MAINCONTROLLER_H