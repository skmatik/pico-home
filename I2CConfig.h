#ifndef I2CCONFIG_H
#define I2CCONFIG_H

#include <hardware/i2c.h>

static constexpr i2c_inst_t *I2C = i2c1;
static constexpr uint32_t SDA = 6;
static constexpr uint32_t SCL = 7;

#endif //I2CCONFIG_H
