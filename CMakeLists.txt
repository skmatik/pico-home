# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BOARD_NAME W5500_EVB_PICO)
set(WIZNET_CHIP W5500)
add_definitions(-D_WIZCHIP_=W5500)
add_definitions(-DDEVICE_BOARD_NAME=W5500_EVB_PICO)


# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-home C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
message(STATUS "PICO_SDK_PATH = ${PICO_SDK_PATH}")

# Set the project root directory if it's not already defined, as may happen if
# the tests folder is included directly by a parent project, without including
# the top level CMakeLists.txt.
if(NOT DEFINED WIZNET_DIR)
    set(WIZNET_DIR ${CMAKE_SOURCE_DIR}/libraries/ioLibrary_Driver)
    message(STATUS "WIZNET_DIR = ${WIZNET_DIR}")
endif()

#if(NOT DEFINED MBEDTLS_DIR)
#    set(MBEDTLS_DIR ${CMAKE_SOURCE_DIR}/libraries/mbedtls)
#    message(STATUS "MBEDTLS_DIR = ${MBEDTLS_DIR}")
#endif()

if(NOT DEFINED PORT_DIR)
    set(PORT_DIR ${CMAKE_SOURCE_DIR}/libraries/WIZNet-PICO-C/port)
    message(STATUS "PORT_DIR = ${PORT_DIR}")
endif()

if(NOT DEFINED I2C_LCD_DIR)
    set(I2C_LCD_DIR ${CMAKE_SOURCE_DIR}/libraries/I2C_LCD)
    message(STATUS "IC2_LCD_DIR = ${I2C_LCD_DIR}")
endif()

if(NOT DEFINED BMP280_I2C_DIR)
    set(BMP280_I2C_DIR ${CMAKE_SOURCE_DIR}/libraries/bmp280_i2c)
    message(STATUS "BMP280_I2C_DIR = ${BMP280_I2C_DIR}")
endif()

if(NOT DEFINED FREERTOS_KERNEL_PATH)
    set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/../FreeRTOS-KernelV11.2.0)
    message(STATUS "FREERTOS_KERNEL_PATH = ${FREERTOS_KERNEL_PATH}")
endif()

# Turn off mbedtls test mode 
set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbedtls programs")
set(ENABLE_TESTING OFF CACHE BOOL "Build mbedtls testing")
add_definitions(-DMBEDTLS_CONFIG_FILE="${PORT_DIR}/mbedtls/inc/ssl_config.h")
add_definitions(-DSET_TRUSTED_CERT_IN_SAMPLES)

# Add libraries in subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/libraries)
#add_subdirectory(${MBEDTLS_DIR})
add_subdirectory(${PORT_DIR})
add_subdirectory(${I2C_LCD_DIR})
add_subdirectory(${BMP280_I2C_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/libraries/onewire_library)

include(FreeRTOS_Kernel_import.cmake)

# Add executable. Default name is the project name, version 0.1

add_executable(pico-home Main.cpp Sensor.cpp MainController.cpp EthernetController.cpp MqttPublisher.cpp MqttFeed.cpp LcdDisplay.cpp
RotaryEncoder.cpp AtmosphericPressureSensor.cpp IndoorTemperatureSensor.cpp LcdMenu.cpp SensorMenuItem.cpp StringMenuItem.cpp
WaterPressureSensor.cpp DS18B20.cpp DS18B20TemperatureSensor.cpp SerialCommunicator.cpp ArduinoTerminalReportBuilder.cpp)

pico_set_program_name(pico-home "pico-home")
pico_set_program_version(pico-home "1.0")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico-home 0)
pico_enable_stdio_usb(pico-home 1)

target_compile_definitions(pico-home PRIVATE configNUMBER_OF_CORES=1)

# Add the standard library to the build
target_link_libraries(pico-home
        pico_stdlib hardware_spi hardware_dma hardware_pio hardware_clocks hardware_i2c hardware_adc
        ETHERNET_FILES
        IOLIBRARY_FILES
        MQTT_FILES
        TIMER_FILES		
        ETHERNET_FILES
        DHCP_FILES
        DNS_FILES
        I2C_LCD_FILES
        bmp280_i2c
        FreeRTOS-Kernel-Heap4
        onewire_library
        )

# Add the standard include files to the build
target_include_directories(pico-home PRIVATE
        ${CMAKE_CURRENT_LIST_DIR} ${PORT_DIR}        
)

pico_add_extra_outputs(pico-home)
pico_set_float_implementation(pico-home pico)
pico_set_double_implementation(pico-home pico)

if(DEFINED ENV{MQTT_PASSWORD})
    message(STATUS "Using environment variable: $ENV{MQTT_PASSWORD}")
    add_compile_definitions(MQTT_PASSWORD=\"$ENV{MQTT_PASSWORD}\")
else()
    message(WARNING "MQTT_PASSWORD is not set, using default value.")
    add_compile_definitions(MQTT_PASSWORD="DefaultValue")
endif()

# Set compile options
add_compile_options(
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )


