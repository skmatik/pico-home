#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/watchdog.h"

#include "MainController.h"
#include "StringMenuItem.h"
#include "ArduinoTerminalReportBuilder.h"
#include "SystemTemperatureSensor.h"

MainController::MainController(): lcdDisplay(LcdDisplay()),
                                  rotaryEncoder(RotaryEncoder()),
                                  ethernetController(EthernetController()),
                                  bmp280I2c(BMP280I2C()),
                                  ds18b20(DS18B20()),

                                  feeds(std::make_shared<std::vector<std::unique_ptr<MQTTFeed> > >()),
                                  menuItems(std::make_shared<std::vector<std::shared_ptr<MenuItem> > >()),
                                  sensors(std::make_shared<std::vector<std::shared_ptr<Sensor> > >()),

                                  lcdMenu(LcdMenu(lcdDisplay, menuItems, 4)),

                                  mqttPublisher(MQTTPublisher(feeds, ethernetController)),
                                  arduinoTerminalReportBuilder(ArduinoTerminalReportBuilder(sensors)),
                                  remoteSensor(std::make_shared<RemoteSensor>("Obyvacka", "C")),
                                  serialCommunicator(SerialCommunicator(arduinoTerminalReportBuilder, remoteSensor)) {
    inactiveMiliseconds = 0;
}

MainController::~MainController() {
    // Destructor implementation (if needed)
}

void MainController::initialize() {
std:
    cout << "Pico Home v1 (c) skmatik" << std::endl;

    lcdDisplay.print("Pico Home v1 skmatik");
    bool connected = ethernetController.initialize();

    lcdDisplay.nextRow();
    if (connected) {
        lcdDisplay.print("IP: " + ethernetController.getIPAddress());
        std::unique_ptr<uint8_t[]> ip = ethernetController.resolveDomain((uint8_t *) "io.adafruit.com");
        if (ip != nullptr) {
            lcdDisplay.nextRow();
            lcdDisplay.print(
                "MQTT: " + std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." + std::to_string(ip[2]) + "." +
                std::to_string(ip[3]));
        }
    } else {
        lcdDisplay.print("Disconnected!");
    }

    if (watchdog_caused_reboot()) {
        std::cout << "Rebooted by Watchdog!" << std::endl;
        mqttPublisher.publishMessage(REBOOT_FEED_NAME, REBOOT_WATCHDOG);
    } else {
        std::cout << "Clean boot." << std::endl;
        mqttPublisher.publishMessage(REBOOT_FEED_NAME, REBOOT_CLEAN);
    }


    initializeSensors();
    watchdog_enable(5000, 1);

    xTaskCreate(MainController::execute, "MainThread", MAIN_TASK_STACK_SIZE, (void *) this, MAIN_TASK_PRIORITY,
                &mainTask);
    xTaskCreate(MainController::updateSensors, "SensorThread", SENSOR_TASK_STACK_SIZE, (void *) this,
                SENSOR_TASK_PRIORITY, &sensorTask);

    serialCommunicator.setupCommunication();
}

void MainController::initializeSensors() {
    std::cout << "Initializing sensors!" << std::endl;
    std::shared_ptr<Sensor> sensor = std::make_shared<DS18B20TemperatureSensor>(
        DS18B20TemperatureSensor(ds18b20, "Kotol", 0x360000062718bd28));
    sensors->push_back(sensor);
    feeds->push_back(std::make_unique<MQTTFeed>("heater", sensor));

    sensor = std::make_shared<
        DS18B20TemperatureSensor>(DS18B20TemperatureSensor(ds18b20, "Bojler", 0x7900000608343428));
    sensors->push_back(sensor);
    feeds->push_back(std::make_unique<MQTTFeed>("water-heater", sensor));

    sensor = std::make_shared<DS18B20TemperatureSensor>(
        DS18B20TemperatureSensor(ds18b20, "Spiatocka", 0x6a00000607fcab28));
    sensors->push_back(sensor);
    feeds->push_back(std::make_unique<MQTTFeed>("heater-feedback", sensor));

    sensor = std::make_shared<WaterPressureSensor>(WaterPressureSensor());
    sensors->push_back(sensor);
    feeds->push_back(std::make_unique<MQTTFeed>("water-pressure", sensor));

    sensor = std::make_shared<AtmosphericPressureSensor>(AtmosphericPressureSensor(bmp280I2c));
    sensors->push_back(sensor);
    //feeds->push_back(std::make_unique<MQTTFeed>("atmospheric-pressure", sensor));

    sensor = std::make_shared<IndoorTemperatureSensor>(IndoorTemperatureSensor(bmp280I2c));
    sensors->push_back(sensor);
    //feeds->push_back(std::make_unique<MQTTFeed>("basement-temperature", sensor));

    //sensors->push_back(remoteSensor);
    //feeds->push_back(std::make_unique<MQTTFeed>("livingroom-temperature", remoteSensor));

    sensor = std::make_shared<SystemTemperatureSensor>(SystemTemperatureSensor());
    feeds->push_back(std::make_unique<MQTTFeed>("system-temperature", sensor));
}

void MainController::initializeMenu() {
    std::cout << "Initializing menu!" << sensors << std::endl;
    for (auto sensor: *sensors) {
        menuItems->push_back(std::make_shared<SensorMenuItem>(sensor));
    }
    lcdMenu.printMenu(true);
}

void MainController::updateSensors(__unused void *parameters) {
    MainController *mainController = (MainController *) parameters;
    while (true) {
        cout << "Updating sensors: " << mainController->sensors->size() << std::endl;
        for (std::shared_ptr<Sensor> sensor: *mainController->sensors) {
            sensor->read();
            //std::cout << sensor->getSensorName() << ": " << sensor->getFormattedValue() << " " << sensor->getUnitsOfMeasurement() << std::endl;
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void MainController::execute(__unused void *parameters) {
    MainController *mainController = (MainController *) parameters;
    mainController->initializeMenu();
    while (true) {
        watchdog_update();
        mainController->loop();
        vTaskDelay(pdMS_TO_TICKS(MAIN_LOOP_REFRESH_INTERVAL_IN_MILLIS));
    }
}

void MainController::loop() {
    bool backlightEnabled;
    if (inactiveMiliseconds > INACTIVE_DISPLAY_INTERVAL_IN_MILLIS) {
        backlightEnabled = false;
    } else {
        backlightEnabled = true;
        inactiveMiliseconds += MAIN_LOOP_REFRESH_INTERVAL_IN_MILLIS;
    }

    if (rotaryEncoder.triggered()) {
        backlightEnabled = true;
        inactiveMiliseconds = 0;
        std::cout << "Switch: " << rotaryEncoder.getSwitched();
        std::cout << " Direction: " << rotaryEncoder.getDirection();
        std::cout << " Counter: " << rotaryEncoder.getCounter();
        std::cout << " item: " << lcdMenu.getCurrentItemIndex();
        std::cout << std::endl;
        rotaryEncoder.resetSwitch();
        if (rotaryEncoder.getDirection() == Direction::LEFT) {
            lcdMenu.previous();
        } else if (rotaryEncoder.getDirection() == Direction::RIGHT) {
            lcdMenu.next();
        }
    }
    lcdMenu.printMenu(backlightEnabled);
}

const std::string MainController::REBOOT_FEED_NAME = "reboot";
const std::string MainController::REBOOT_CLEAN = "1";
const std::string MainController::REBOOT_WATCHDOG = "2";
