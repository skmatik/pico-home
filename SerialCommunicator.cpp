#include <cstring>
#include <iostream>
#include "SerialCommunicator.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

SerialCommunicator::SerialCommunicator(ArduinoTerminalReportBuilder &reportBuilder) : reportBuilder(reportBuilder)
{
}

void SerialCommunicator::execute(__unused void *params)
{
    SerialCommunicator *serialCommunicator = (SerialCommunicator *)params;
    while (true)
    {

        std::cout << "Serial: execute" << std::endl;
        if (commandComplete)
        {
            std::cout << "Serial: " << command << std::endl;
            if (command == "{S@")
            {
                serialCommunicator->writeResponse();
                commandComplete = false;
                command.clear();
            }
            else if (command.substr(0, 2) == "}T")
            {
                if (commandComplete)
                {
                    std::cout << " temp from arduino: " << command.substr(2) << std::endl;
                    // remoteSensor->updateTemperatureString(command);
                    commandComplete = false;
                    command.clear();
                }
            }
            else
            {
                commandComplete = false;
                command.clear();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
        command = "{S@";
        commandComplete = true;
    }
}

void SerialCommunicator::setupCommunication()
{
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(uart_id, false, false);

    // Set our data format
    uart_set_format(uart_id, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(uart_id, false);

    // Set up our UART with the required speed.
    uart_init(uart_id, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

    int UART_IRQ = uart_id == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    //irq_set_exclusive_handler(UART_IRQ, SerialCommunicator::readUart);
    //irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    //uart_set_irq_enables(uart_id, true, false);

    xTaskCreate(SerialCommunicator::execute, "SerialCommunicatorThread", SERIAL_COMMUNICATION_TASK_STACK_SIZE, (void *)this, SERIAL_COMMUNICATION_TASK_PRIORITY, &task);
    std::cout << "Serial: initialized" << std::endl;
}

void SerialCommunicator::readUart()
{
    std::cout << "IRQ" << std::endl;
    std::string text;
    while (uart_is_readable(uart1))
    {
        std::cout << "IRQ1" << std::endl;
        uint8_t ch = uart_getc(uart1);
        std::cout << "IRQ2" << std::endl;
        text.push_back(ch);
        std::cout << "IRQ3" << std::endl;
        // Can we send it back?
        uart_putc(uart0, ch);
        std::cout << "IRQ4" << std::endl;
        if (ch == '@')
        {
            std::cout << "IRQ5" << std::endl;
            commandComplete = true;
            break;
        }
    }
    std::cout << "IRQ6" << std::endl;
    command.append(text);
    std::cout << "IRQ7" << std::endl;

}

void SerialCommunicator::writeResponse()
{
    const std::vector<std::string> report = reportBuilder.build();
    for (const auto &line : report)
    {
        sendLine(line);
        sendChar('$');
        // TODO: maybe needed: delay(100);
    }
    sendChar('@');
}

void SerialCommunicator::sendLine(std::string_view line)
{
    // Send out a string, with CR/LF conversions
    uart_puts(uart_id, line.data());
}

void SerialCommunicator::sendChar(char c)
{
    uart_putc(uart_id, c);
}

uart_inst_t *SerialCommunicator::uart_id = uart1;
bool SerialCommunicator::commandComplete = false;
std::string SerialCommunicator::command = std::string();