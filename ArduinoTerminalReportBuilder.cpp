#include <sstream>
#include <utility>
#include "ArduinoTerminalReportBuilder.h"
#include <memory>
#include <iostream>

std::vector<std::string> ArduinoTerminalReportBuilder::build() {
    std::vector<std::string> report = std::vector<std::string>();
    for (std::shared_ptr<Sensor> sensor : *sensors)
    {
        std::string line = std::string(sensor->getSensorName());
        line.append("-");
        line.append(sensor->getFormattedValue());
        report.push_back(line);
    }
    return report;
}

ArduinoTerminalReportBuilder::ArduinoTerminalReportBuilder(std::shared_ptr<std::vector<std::shared_ptr<Sensor>>> sensors) : sensors(sensors) {
}
