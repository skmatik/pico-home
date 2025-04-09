#ifndef ARDUINOTERMINALREPORTBUILDER_H
#define ARDUINOTERMINALREPORTBUILDER_H

#include "Sensor.h"
#include <vector>
#include <string>
#include <memory>

class ArduinoTerminalReportBuilder
{

public:
    ArduinoTerminalReportBuilder(std::shared_ptr<std::vector<std::shared_ptr<Sensor>>> sensors);

    std::vector<std::string> build();

private:
    std::shared_ptr<std::vector<std::shared_ptr<Sensor>>> sensors;
};

#endif // ARDUINOTERMINALREPORTBUILDER_H
