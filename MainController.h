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
    static constexpr int INACTIVE_DISPLAY_INTERVAL_IN_MILLIS = 15 * ONE_SECOND_IN_MILLISECONDS;
    static constexpr int MAIN_LOOP_REFRESH_INTERVAL_IN_MILLIS = 100;

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
    
    MQTTPublisher mqttPublisher;
    ArduinoTerminalReportBuilder arduinoTerminalReportBuilder;
    std::shared_ptr<RemoteSensor> remoteSensor;
    SerialCommunicator serialCommunicator;

    TaskHandle_t mainTask;
    TaskHandle_t sensorTask;
    int inactiveMiliseconds;

    static void updateSensors(__unused void *params);
    static void execute(__unused void *params);
    void loop();
    void initializeMenu();
    void initializeSensors();

public:
    MainController();
    ~MainController();
    void initialize();
};

#endif // MAINCONTROLLER_H