#include "LcdDisplay.h"

LcdDisplay::LcdDisplay():lcd(LCD_I2C(I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS, I2C, SDA, SCL))
{
    bi_decl(bi_1pin_with_name(SDA, "[SDA] LCD screen serial data pin"))
    bi_decl(bi_1pin_with_name(SCL, "[SCL] LCD screen serial clock pin"))

    lcd.BacklightOn();
    lcd.Clear();
    lcd.SetCursor(0, 0);
}

void LcdDisplay::nextRow()
{
    rowNumber++;
    if (rowNumber >= LCD_ROWS)
    {
        rowNumber = 0;
    }
    lcd.SetCursor(rowNumber, 0);
}

void LcdDisplay::beginOfLine()
{
    lcd.SetCursor(rowNumber, 0);
}

void LcdDisplay::print(std::string_view str)
{
    lcd.PrintString(str.data());
}

void LcdDisplay::clear()
{
    lcd.Clear();
    lcd.SetCursor(0, 0);
}

void LcdDisplay::configureBackLight(bool enabled)
{
    if (enabled)
    {
        lcd.BacklightOn();
    }
    else
    {
        lcd.BacklightOff();
    }
}

void LcdDisplay::topLeft()
{
    lcd.SetCursor(0, 0);
}