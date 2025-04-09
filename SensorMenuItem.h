#ifndef SENSORMENUITEM_H
#define SENSORMENUITEM_H

#include <string>
#include <memory>

#include "MenuItem.h"
#include "Sensor.h"

class SensorMenuItem: public MenuItem {

private:
    std::shared_ptr<Sensor> sensor;
    std::string itemText;
public:
    explicit SensorMenuItem(std::shared_ptr<Sensor> sensor);
    std::string_view getItemText() override;
};


#endif //SENSORMENUITEM_H
