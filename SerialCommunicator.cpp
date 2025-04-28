#include <cstring>
#include <iostream>
#include "SerialCommunicator.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

SerialCommunicator::SerialCommunicator(ArduinoTerminalReportBuilder &reportBuilder, std::shared_ptr<RemoteSensor> remoteSensor) : reportBuilder(reportBuilder), remoteSensor(std::move(remoteSensor))
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
            std::cout << "Serial command: " << command << std::endl;
            if (command == "{S@")
            {
                serialCommunicator->writeResponse();
                commandComplete = false;
                command.clear();
            }
            else if (command.substr(0, 2) == "}T")
            {
                std::cout << "Temp from arduino: " << command.substr(2) << std::endl;
                serialCommunicator->remoteSensor->updateTemperatureString(command);
                commandComplete = false;
                command.clear();
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

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, false);

    xTaskCreate(SerialCommunicator::execute, "SerialCommunicatorThread", SERIAL_COMMUNICATION_TASK_STACK_SIZE, (void *)this, SERIAL_COMMUNICATION_TASK_PRIORITY, &task);
    std::cout << "Serial: initialized" << std::endl;
}

void SerialCommunicator::readUart()
{
    std::string text;
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    while (uart_is_readable(uart1))
    {
        uint8_t ch = uart_getc(uart1);
        text.push_back(ch);
        if (ch == '@')
        {
            gpio_put(PICO_DEFAULT_LED_PIN, false);
            commandComplete = true;
            break;
        }
    }
    command.append(text);
}

void SerialCommunicator::writeResponse()
{
    const std::vector<std::string> report = reportBuilder.build();
    for (const auto &line : report)
    {
        sendLine(line);
        sendChar('$');
        // TODO: maybe needed: delay(100);
        std::cout << "Serial: " << line << std::endl;
        vTaskDelay(pdMS_TO_TICKS(100));
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