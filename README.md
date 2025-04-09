# Pico Home
Raspberry PI Pico IOT Home Automation.

Raspberry PI Pico tasks
- collects data from all sensors
- display data from sensor to LCD display
- publish data using MQTT to server
- react to rotary encoder rotation events (navigate on display)
- react to rotary encoder button events (perform actions)
- communicate with Arduino (act as remote terminal) using serial protocol

## Current status
- EthernetController is able to do DHCP, DNS resolving
- Added RotaryEncoder
- Added BMP280 Sensor
- Added LCD display
- Sensors are publishing to MQTT (io.adafruit.com)
- LCD display is printing sensors
- Rotary encoder is navigating on LCD display
- FreeRTOS integrated
- Serial communication with Arduino added

## setup
- Environment variable MQTT_PASSWORD should contain password during compilation
- switch ioLibrary_Driver to tag 3.2.0


## TODO
- libraries/I2C_LCD - make a fork from https://github.com/cristiancristea00/Pico-I2C-LCD/tree/main and create library, add it as submodule to libraries folder
- libraries/WIZnet-PICO-C/port
    - if possible create submodule with only port folder from https://github.com/WIZnet-ioNIC/WIZnet-PICO-C
    - modified files: timer.h, w5x00_spi.h - added extern C
- libraries/ioLibrary_Driver
    - FTPClient and TFTP not compilable, removed from libaries/CMakeLists.txt
- libraries/bmp280_i2c - copy from pico examples
- libraries/onewire_library - copy from prico examples - added extern C
- Ethernet - detect disconnection
- Rotary encoder - finetune, create library
- IC2
    - extract configuration
    - extract initialization
- BMP280 - refactor, document, create library
- Document FreeRTOS setup
- Create logger
- BMP280 Sensor is hanging
- Serial communication is oneway