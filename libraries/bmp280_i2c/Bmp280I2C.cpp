/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "Bmp280I2C.h"
#include <iostream>
#include "I2CConfig.h"

/* BMP280 I2C temperature and pressure sensor

   GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on BMP280
   board
   GPIO PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7)) -> SCL on
   BMP280 board
   3.3v (pin 36) -> VCC on BMP280 board
   GND (pin 38)  -> GND on BMP280 board
*/

BMP280I2C::BMP280I2C()
{
    std::cout << "Initializing BMP280" << std::endl;

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(I2C, 100 * 1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    initialize();

    // retrieve fixed compensation params
    getCalibrationParams();
}

BMP280I2C::~BMP280I2C()
{
}

void BMP280I2C::initialize()
{
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // 500ms sampling time, x16 filter
    const uint8_t reg_config_val = ((0x04 << 5) | (0x05 << 2)) & 0xFC;

    // send register number followed by its corresponding value
    buf[0] = REG_CONFIG;
    buf[1] = reg_config_val;
    i2c_write_blocking(i2c1, ADDR, buf, 2, false);

    // osrs_t x1, osrs_p x4, normal mode operation
    const uint8_t reg_ctrl_meas_val = (0x01 << 5) | (0x03 << 2) | (0x03);
    buf[0] = REG_CTRL_MEAS;
    buf[1] = reg_ctrl_meas_val;
    i2c_write_blocking(i2c1, ADDR, buf, 2, false);
}


void BMP280I2C::readRawData(int32_t *temp, int32_t *pressure)
{
    // BMP280 data registers are auto-incrementing and we have 3 temperature and
    // pressure registers each, so we start at 0xF7 and read 6 bytes to 0xFC
    // note: normal mode does not require further ctrl_meas and config register writes
    std::cout << "read raw data" << std::endl;
    uint8_t buf[6];
    uint8_t reg = REG_PRESSURE_MSB;
    std::cout << "read raw data1" << std::endl;
    i2c_write_blocking(i2c1, ADDR, &reg, 1, true); // true to keep master control of bus
    std::cout << "read raw data2" << std::endl;
    i2c_read_blocking(i2c1, ADDR, buf, 6, false);  // false - finished with bus
    std::cout << "read raw data3" << std::endl;

    // store the 20 bit read in a 32 bit signed integer for conversion
    *pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    std::cout << "read raw data4" << std::endl;
    *temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
    std::cout << "read raw data5" << std::endl;

}

void BMP280I2C::reset()
{
    // reset the device with the power-on-reset procedure
    uint8_t buf[2] = {REG_RESET, 0xB6};
    i2c_write_blocking(i2c1, ADDR, buf, 2, false);
}

// intermediate function that calculates the fine resolution temperature
// used for both pressure and temperature conversions
int32_t BMP280I2C::convert(int32_t temp)
{
    // use the 32-bit fixed point compensation implementation given in the
    // datasheet

    int32_t var1, var2;
    var1 = ((((temp >> 3) - ((int32_t)digT1 << 1))) * ((int32_t)digTX[0])) >> 11;
    var2 = (((((temp >> 4) - ((int32_t)digT1)) * ((temp >> 4) - ((int32_t)digT1))) >> 12) * ((int32_t)digTX[1])) >> 14;
    return var1 + var2;
}

int32_t BMP280I2C::convertTemperature(int32_t temp)
{
    // uses the BMP280 calibration parameters to compensate the temperature value read from its registers
    int32_t t_fine = convert(temp);
    return (t_fine * 5 + 128) >> 8;
}

int32_t BMP280I2C::convertPressure(int32_t pressure, int32_t temp)
{
    // uses the BMP280 calibration parameters to compensate the pressure value read from its registers

    int32_t t_fine = convert(temp);

    int32_t var1, var2;
    uint32_t converted = 0.0;
    var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)digPX[4]);
    var2 += ((var1 * ((int32_t)digPX[3])) << 1);
    var2 = (var2 >> 2) + (((int32_t)digPX[2]) << 16);
    var1 = (((digPX[1] * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)digPX[0]) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)digP1)) >> 15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    converted = (((uint32_t)(((int32_t)1048576) - pressure) - (var2 >> 12))) * 3125;
    if (converted < 0x80000000)
    {
        converted = (converted << 1) / ((uint32_t)var1);
    }
    else
    {
        converted = (converted / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)digPX[7]) * ((int32_t)(((converted >> 3) * (converted >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(converted >> 2)) * ((int32_t)digPX[6])) >> 13;
    converted = (uint32_t)((int32_t)converted + ((var1 + var2 + digPX[5]) >> 4));
    return converted;
}

void BMP280I2C::getCalibrationParams()
{
    // raw temp and pressure values need to be calibrated according to
    // parameters generated during the manufacturing of the sensor
    // there are 3 temperature params, and 9 pressure params, each with a LSB
    // and MSB register, so we read from 24 registers

    uint8_t buf[NUM_CALIB_PARAMS] = {0};
    uint8_t reg = REG_DIG_T1_LSB;
    i2c_write_blocking(i2c1, ADDR, &reg, 1, true); // true to keep master control of bus
    // read in one go as register addresses auto-increment
    i2c_read_blocking(i2c1, ADDR, buf, NUM_CALIB_PARAMS, false); // false, we're done reading

    // store these in a struct for later use
    digT1 = (uint16_t) (buf[1] << 8) | buf[0];
    digTX[0] = (int16_t) (buf[3] << 8) | buf[2];
    digTX[1] = (int16_t) (buf[5] << 8) | buf[4];

    digP1 = (uint16_t) (buf[7] << 8) | buf[6];
    digPX[0] = (int16_t) (buf[9] << 8) | buf[8];
    digPX[1] = (int16_t) (buf[11] << 8) | buf[10];
    digPX[2] = (int16_t) (buf[13] << 8) | buf[12];
    digPX[3] = (int16_t) (buf[15] << 8) | buf[14];
    digPX[4] = (int16_t) (buf[17] << 8) | buf[16];
    digPX[5] = (int16_t) (buf[19] << 8) | buf[18];
    digPX[6] = (int16_t) (buf[21] << 8) | buf[20];
    digPX[7] = (int16_t) (buf[23] << 8) | buf[22];
}

void BMP280I2C::print()
{
    std::cout << "Pressure = " << getPressureInkPa() << " kPa" << std::endl;
    std::cout << "Temperature = " << getTemperatureInDegC() << " C" << std::endl;
}

double BMP280I2C::getTemperatureInDegC()
{
    //readRawData(&rawTemperature, &rawPressure);
    return convertTemperature(rawTemperature) / 100.f;
}

double BMP280I2C::getPressureInkPa()
{
    //readRawData(&rawTemperature, &rawPressure);
    return convertPressure(rawPressure, rawTemperature) / 1000.f;
}
