# I2C_LCD
add_library(I2C_LCD_FILES STATIC)

target_sources(I2C_LCD_FILES PUBLIC
        ${I2C_LCD_DIR}/LCD_I2C.cpp
        )

target_include_directories(I2C_LCD_FILES PUBLIC
        ${I2C_LCD_DIR}/
        )

target_link_libraries(I2C_LCD_FILES pico_stdlib hardware_i2c)