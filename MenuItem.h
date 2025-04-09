#ifndef MENUITEM_H
#define MENUITEM_H
#include <string>

class MenuItem {
public:
    virtual std::string_view getItemText() = 0;
};

#endif //MENUITEM_H
