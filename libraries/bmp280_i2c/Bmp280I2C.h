#ifndef BMP280_I2C_H
#define BMP280_I2C_H

#define I2C_SDA 6
#define I2C_SCL 7

// device has default bus address of 0x76
#define ADDR _u(0x76)

// hardware registers
#define REG_CONFIG _u(0xF5)
#define REG_CTRL_MEAS _u(0xF4)
#define REG_RESET _u(0xE0)

#define REG_TEMP_XLSB _u(0xFC)
#define REG_TEMP_LSB _u(0xFB)
#define REG_TEMP_MSB _u(0xFA)

#define REG_PRESSURE_XLSB _u(0xF9)
#define REG_PRESSURE_LSB _u(0xF8)
#define REG_PRESSURE_MSB _u(0xF7)

// calibration registers
#define REG_DIG_T1_LSB _u(0x88)
#define REG_DIG_T1_MSB _u(0x89)
#define REG_DIG_T2_LSB _u(0x8A)
#define REG_DIG_T2_MSB _u(0x8B)
#define REG_DIG_T3_LSB _u(0x8C)
#define REG_DIG_T3_MSB _u(0x8D)
#define REG_DIG_P1_LSB _u(0x8E)
#define REG_DIG_P1_MSB _u(0x8F)
#define REG_DIG_P2_LSB _u(0x90)
#define REG_DIG_P2_MSB _u(0x91)
#define REG_DIG_P3_LSB _u(0x92)
#define REG_DIG_P3_MSB _u(0x93)
#define REG_DIG_P4_LSB _u(0x94)
#define REG_DIG_P4_MSB _u(0x95)
#define REG_DIG_P5_LSB _u(0x96)
#define REG_DIG_P5_MSB _u(0x97)
#define REG_DIG_P6_LSB _u(0x98)
#define REG_DIG_P6_MSB _u(0x99)
#define REG_DIG_P7_LSB _u(0x9A)
#define REG_DIG_P7_MSB _u(0x9B)
#define REG_DIG_P8_LSB _u(0x9C)
#define REG_DIG_P8_MSB _u(0x9D)
#define REG_DIG_P9_LSB _u(0x9E)
#define REG_DIG_P9_MSB _u(0x9F)

// number of calibration registers to be read
#define NUM_CALIB_PARAMS 24

class BMP280I2C
{
private:
    // temperature params
    uint16_t digT1;
    int16_t digTX[2];

    // pressure params
    uint16_t digP1;
    int16_t digPX[8];

    int32_t rawTemperature;
    int32_t rawPressure;

    void getCalibrationParams();
    int32_t convert(int32_t temp);
    int32_t convertTemperature(int32_t temp);
    int32_t convertPressure(int32_t pressure, int32_t temp);
    void reset();
    void readRawData(int32_t *temp, int32_t *pressure);

public:
    BMP280I2C();
    ~BMP280I2C();
    void initialize();
    void print();
    double getTemperatureInDegC();
    double getPressureInkPa();
};

#endif
