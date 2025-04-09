#include "LcdMenu.h"

#include <iostream>

bool LcdMenu::next() {
    if (currentItemIndex < (menuItems->size() - rowsVisible - 1)) {
        ++currentItemIndex;
        return true;
    }
    return false;
}

bool LcdMenu::previous() {
    if (currentItemIndex > 0) {
        --currentItemIndex;
        return true;
    }
    return false;
}

int LcdMenu::getCurrentItemIndex() const {
    return currentItemIndex;
}

void LcdMenu::printMenu() {
    int firstItemIndex = getCurrentItemIndex();
    lcdDisplay.topLeft();

    for (int i = 0; i < 4; i++) {
        auto item = menuItems->operator[](i + firstItemIndex).get();
        lcdDisplay.print(item->getItemText());
        lcdDisplay.nextRow();
    }
}

LcdMenu::~LcdMenu() {
}

LcdMenu::LcdMenu(LcdDisplay &lcdDisplay, std::shared_ptr<std::vector<std::shared_ptr<MenuItem>>> menuItems, int rowsVisible)
    : lcdDisplay(lcdDisplay), menuItems(menuItems), rowsVisible(rowsVisible), currentItemIndex(0) {
}

void LcdMenu::setBackLightEnabled(bool enabled) {
    //lcdDriver->setBackLight(enabled);
}
