#ifndef SENSOR_H
#define SENSOR_H

#include <string>

/**
 * @brief Abstract class for sensors that has to be implemented by all sensors.
 */
class Sensor
{
private:
    bool present;
    std::string formattedValue;
    const std::string sensorName;
    const std::string unitsOfMeasurement;

protected:
    static const std::string NA;
    void setPresent(bool present);
    void setFormattedValue(const std::string &formattedValue);
    std::string formatDoubleValue(double value);
public:
    Sensor(const std::string &sensorName, const std::string &getUnitsOfMeasurement);

    /*
     * @brief Get the human readble sensor name.
     * Name should be maximum 8 characters long.
     */
    const std::string &getSensorName() const;

    /*
     * @brief Get the sensor printable value.
     */
    const std::string &getFormattedValue() const;

    /*
     * @brief Get the printable units of measurement applicable for formatted value.
     */
    const std::string &getUnitsOfMeasurement() const;

    /*
     * @brief Read the sensor value and update the formatted value.
     * This function will be called periodically to update the sensor value.
     * If the sensor fails to read a value, the formatted value should remain unchanged.
     */
    virtual void read() = 0;

    /**
     * @brief Check if the sensor is present and working.
     * This function will be called periodically.
     */
    virtual bool isPresent();

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Sensor() = default;
};

#endif // SENSOR_H
