#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "LCD_I2C.h"
#include "pico/stdlib.h"
#include <memory>
#include <iostream>
#include "pico/binary_info/code.h"

class LcdDisplay
{
private:
    static constexpr i2c_inst_t *I2C = i2c1;
    static constexpr uint32_t SDA = 6;
    static constexpr uint32_t SCL = 7;
    static constexpr uint32_t I2C_ADDRESS = 0x27;
    static constexpr uint32_t LCD_COLUMNS = 20;
    static constexpr uint32_t LCD_ROWS = 4;

    LCD_I2C lcd;
    uint rowNumber = 0;

public:
    LcdDisplay();
    void nextRow();
    void beginOfLine();
    void print(std::string_view str);
    void clear();
    void configureBackLight(bool enabled);
    void topLeft();
};

#endif