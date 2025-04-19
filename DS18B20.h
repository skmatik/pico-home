#ifndef DS18B20_H
#define DS18B20_H

#include "onewire.h"

#define DS18B20_CONVERT_T 0x44
#define DS18B20_WRITE_SCRATCHPAD 0x4e
#define DS18B20_READ_SCRATCHPAD 0xbe
#define DS18B20_COPY_SCRATCHPAD 0x48
#define DS18B20_RECALL_EE 0xb8
#define DS18B20_READ_POWER_SUPPLY 0xb4

class DS18B20
{
private:
    static constexpr int ONE_WIRE_GPIO = 22;
    static constexpr int MAX_NUMBER_OF_DEVICES = 10;

    uint offset;
    OW ow;
    uint64_t romCodes[MAX_NUMBER_OF_DEVICES];
    int16_t temperatures[MAX_NUMBER_OF_DEVICES];
    int foundDevices;
    PIO pio;

    void initialize();
    void scanDevices();

public:
    DS18B20();
    ~DS18B20();
    void readTemperatures();
    int16_t getTemperature(uint64_t romCode);
    bool isPresent(uint64_t romCode);
};

#endif // DS18B20_H