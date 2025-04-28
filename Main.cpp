#include "MainController.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include <iostream>
#include "ds18b20.h"
#include "hardware/watchdog.h"

int main()
{
    stdio_init_all();
    sleep_ms(2000); // Wait for the serial port to be ready
    std::cout << "Starting Pico Home..." << std::endl;
    if (watchdog_caused_reboot()) {
        std::cout << "Rebooted by Watchdog!" << std::endl;
    } else {
        std::cout << "Clean boot." << std::endl;
    }

    MainController mainController;
    mainController.initialize();
    vTaskStartScheduler();
    return 0;
}

