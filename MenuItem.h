#ifndef MENUITEM_H
#define MENUITEM_H
#include <string>

/*
 * @brief The MenuItem class is an abstract base class for menu items.
 * Menu items are displayed on an LCD display that is 20 characters wide.
 * The class enforces this constraint by truncating or validating the text.
 */
class MenuItem
{
private:

public:
    virtual std::string& getItemText() = 0;
};

#endif // MENUITEM_H
