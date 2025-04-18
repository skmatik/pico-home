#ifndef STRINGMENUITEM_H
#define STRINGMENUITEM_H

#include <string>

#include "MenuItem.h"

class StringMenuItem : public MenuItem {

    std::string text;
public:
    explicit StringMenuItem(std::string_view text);
    std::string& getItemText() override;
};


#endif //HOMEPI_STRINGMENUITEM_H
