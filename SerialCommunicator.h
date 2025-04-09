#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

#include <memory>
#include "hardware/uart.h"
#include "FreeRTOS.h"
#include "task.h"

#include "ArduinoTerminalReportBuilder.h"
#include "TimeConstants.h"

class SerialCommunicator
{

private:
    static constexpr int SERIAL_COMMUNICATION_TASK_STACK_SIZE = configMINIMAL_STACK_SIZE;
    static constexpr int SERIAL_COMMUNICATION_TASK_PRIORITY = tskIDLE_PRIORITY;

    static constexpr int BAUD_RATE = 9600;
    static constexpr int DATA_BITS = 8;
    static constexpr int STOP_BITS = 1;
    static constexpr uart_parity_t PARITY = UART_PARITY_NONE;

    static constexpr int UART_TX_PIN = 8;
    static constexpr int UART_RX_PIN = 9;

    static uart_inst_t* uart_id;
    static std::string command;
    static bool commandComplete;
    ArduinoTerminalReportBuilder reportBuilder;
    TaskHandle_t task;

    static void readUart();
    static void execute(__unused void *params);

    void writeResponse();
    void sendLine(std::string_view line);
    void sendChar(char c);

public:
    SerialCommunicator(ArduinoTerminalReportBuilder& reportBuilder);
    void setupCommunication();
};

#endif // SERIALCOMMUNICATOR_H
