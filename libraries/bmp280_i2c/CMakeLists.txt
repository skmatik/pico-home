add_library(bmp280_i2c INTERFACE)
target_sources(bmp280_i2c INTERFACE ${BMP280_I2C_DIR}/Bmp280I2C.cpp)

target_link_libraries(bmp280_i2c INTERFACE
        pico_stdlib
        )

# add the `binary` directory so that the generated headers are included in the project
#
target_include_directories(bmp280_i2c INTERFACE
	${BMP280_I2C_DIR}
	)
