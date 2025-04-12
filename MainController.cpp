#include "MainController.h"
#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "StringMenuItem.h"
#include "ArduinoTerminalReportBuilder.h"

MainController::MainController(): lcdDisplay(LcdDisplay()),
rotaryEncoder(RotaryEncoder()),
ethernetController(EthernetController()),
bmp280I2c(BMP280I2C()),
ds18b20(DS18B20()),

feeds(std::make_shared<std::vector<std::unique_ptr<MQTTFeed>>>()),
menuItems(std::make_shared<std::vector<std::shared_ptr<MenuItem>>>()),
sensors(std::make_shared<std::vector<std::shared_ptr<Sensor>>>()),

lcdMenu(LcdMenu(lcdDisplay, menuItems, 4)),
atmosphericPressureSensor(AtmosphericPressureSensor(bmp280I2c)),
indoorTemperatureSensor(IndoorTemperatureSensor(bmp280I2c)),
waterPressureSensor(WaterPressureSensor()),
ds18b20TemperatureSensor(DS18B20TemperatureSensor(ds18b20, "Sonda1", 0xd6011591ac3fff28)),

mqttPublisher(MQTTPublisher(feeds, ethernetController)),
arduinoTerminalReportBuilder(ArduinoTerminalReportBuilder(sensors)),
serialCommunicator(SerialCommunicator(arduinoTerminalReportBuilder))
{
}

MainController::~MainController()
{
    // Destructor implementation (if needed)
}

void MainController::initialize()
{
std:
    cout << "Pico Home v1 (c) skmatik" << std::endl;

    lcdDisplay.print("Pico Home v1 skmatik");
    bool connected = ethernetController.initialize();

    lcdDisplay.nextRow();
    if (connected)
    {
        lcdDisplay.print("IP: " + ethernetController.getIPAddress());
        std::unique_ptr<uint8_t[]> ip = ethernetController.resolveDomain((uint8_t *)"io.adafruit.com");
        if (ip != nullptr)
        {
            lcdDisplay.nextRow();
            lcdDisplay.print("MQTT: " + std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." + std::to_string(ip[2]) + "." + std::to_string(ip[3]));
        }
    }
    else
    {
        lcdDisplay.print("Disconnected!");
    }

    sensors->push_back(std::make_shared<AtmosphericPressureSensor>(atmosphericPressureSensor));
    sensors->push_back(std::make_shared<IndoorTemperatureSensor>(indoorTemperatureSensor));
    sensors->push_back(std::make_shared<WaterPressureSensor>(waterPressureSensor));
    sensors->push_back(std::make_shared<DS18B20TemperatureSensor>(ds18b20TemperatureSensor));

    xTaskCreate(MainController::execute, "MainThread", MAIN_TASK_STACK_SIZE, (void *)this, MAIN_TASK_PRIORITY, &mainTask);
    xTaskCreate(MainController::updateSensors, "SensorThread", SENSOR_TASK_STACK_SIZE, (void *)this, SENSOR_TASK_PRIORITY, &sensorTask);

    feeds->push_back(std::make_unique<MQTTFeed>("atmospheric-pressure", atmosphericPressureSensor));
    feeds->push_back(std::make_unique<MQTTFeed>("basement-temperature", indoorTemperatureSensor));
    feeds->push_back(std::make_unique<MQTTFeed>("water-pressure", waterPressureSensor));
    feeds->push_back(std::make_unique<MQTTFeed>("room-temperature", ds18b20TemperatureSensor));

    serialCommunicator.setupCommunication();
}

void MainController::initializeMenu()
{
    std::cout << "Initializing menu!" << sensors << std::endl;
    for (auto sensor : *sensors)
    {
        menuItems->push_back(std::make_shared<SensorMenuItem>(sensor));
    }
    menuItems->push_back(std::make_unique<StringMenuItem>("item0"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item1"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item2"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item3"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item4"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item5"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item6"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item7"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item8"));
    menuItems->push_back(std::make_unique<StringMenuItem>("item9"));
    lcdMenu.printMenu();
}

void MainController::updateSensors(__unused void *parameters)
{
    MainController *mainController = (MainController *)parameters;
    while (true)
    {

        cout << "Updating sensors: " << mainController->sensors->size() << std::endl;
        for (std::shared_ptr<Sensor> sensor : *mainController->sensors)
        {
            sensor->read();
            std::cout << sensor->getSensorName() << ": " << sensor->getFormattedValue() << " " << sensor->getUnitsOfMeasurement() << std::endl;
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void MainController::execute(__unused void *parameters)
{
    MainController *mainController = (MainController *)parameters;
    mainController->initializeMenu();
    while (true)
    {
        mainController->loop();
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 100 ms
    }
}

void MainController::loop()
{
    if (rotaryEncoder.triggered())
    {
        lcdDisplay.configureBackLight(true);
        // nextTurnOffTime = millis() + INACTIVE_DISPLAY_INTERVAL_IN_MILLIS;
        std::cout << "Switch: " << rotaryEncoder.getSwitched();
        std::cout << " Direction: " << rotaryEncoder.getDirection();
        std::cout << " Counter: " << rotaryEncoder.getCounter();
        std::cout << " item: " << lcdMenu.getCurrentItemIndex();
        std::cout << std::endl;
        rotaryEncoder.resetSwitch();
        if (rotaryEncoder.getDirection() == Direction::LEFT)
        {
            lcdMenu.previous();
        }
        else if (rotaryEncoder.getDirection() == Direction::RIGHT)
        {
            lcdMenu.next();
        }
    }
    lcdMenu.printMenu();
}