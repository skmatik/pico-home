#include "DS18B20.h"
#include <iostream>

DS18B20::DS18B20()
{
    std::cout << "DS18B20" << std::endl;
    foundDevices = 0;
    pio = pio0;
    initialize();
}

DS18B20::~DS18B20()
{
    // Destructor implementation (if needed)
}

void DS18B20::initialize()
{
    // Initialize the 1-wire driver
    if (pio_can_add_program(pio, &onewire_program))
    {
        offset = pio_add_program(pio, &onewire_program);
        if (!ow_init(&ow, pio, offset, ONE_WIRE_GPIO))
        {
            std::cout << "Could not initialise the One Wire driver" << std::endl;
        }
    }
    else
    {
        std::cout << "Could not add PIO One Wire program" << std::endl;
    }
}

void DS18B20::scanDevices() {
    
    foundDevices = ow_romsearch(&ow, romCodes, MAX_NUMBER_OF_DEVICES, OW_SEARCH_ROM);
    std::cout << "Found " << foundDevices << " D18B20 devices" << std::endl;
}

void DS18B20::readTemperatures() {
    scanDevices();
    if (foundDevices > 0)
    {
        // start temperature conversion in parallel on all devices
        // (see ds18b20 datasheet)
        ow_reset(&ow);
        ow_send(&ow, OW_SKIP_ROM);
        ow_send(&ow, DS18B20_CONVERT_T);

        // wait for the conversions to finish
        while (ow_read(&ow) == 0)
            ;

        // read the result from each device
        for (int i = 0; i < foundDevices; i += 1)
        {
            ow_reset(&ow);
            ow_send(&ow, OW_MATCH_ROM);
            for (int b = 0; b < 64; b += 8)
            {
                ow_send(&ow, romCodes[i] >> b);
            }
            ow_send(&ow, DS18B20_READ_SCRATCHPAD);
            int16_t temp = 0;
            temp = ow_read(&ow) | (ow_read(&ow) << 8);
            temperatures[i] = temp;
        }
    }
}

int16_t DS18B20::getTemperature(uint64_t romCode)
{
    for (int i = 0; i < foundDevices; i++)
    {
        if (romCodes[i] == romCode)
        {
            return temperatures[i];
        }
    }
    return 0;
}
