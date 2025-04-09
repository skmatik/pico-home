#ifndef LCDMENU_H
#define LCDMENU_H
#include <vector>
#include <string>
#include "MenuItem.h"
#include "LcdDisplay.h"

class LcdMenu {
private:
    std::shared_ptr<std::vector<std::shared_ptr<MenuItem>>> menuItems;
    int currentItemIndex{};
    int rowsVisible;
    LcdDisplay lcdDisplay;
public:
    LcdMenu(LcdDisplay &lcdDisplay, std::shared_ptr<std::vector<std::shared_ptr<MenuItem>>> menuItems, int rowsVisible);

    virtual ~LcdMenu();

    bool next();
    bool previous();
    int getCurrentItemIndex() const;
    void printMenu();
    void setBackLightEnabled(bool enabled);
};


#endif //LCDMENU_H
