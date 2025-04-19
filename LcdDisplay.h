#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "LCD_I2C.h"
#include "pico/stdlib.h"
#include <memory>
#include <iostream>
#include "pico/binary_info/code.h"
#include "I2CConfig.h"

class LcdDisplay
{
private:
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