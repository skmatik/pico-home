#include "StringMenuItem.h"

StringMenuItem::StringMenuItem(std::string_view text) {
    unsigned int textSize = text.size();
    this->text.clear();
    this->text.append(text);
    this->text.append(20 - textSize, ' ');
}

std::string& StringMenuItem::getItemText() {
    return text;
}
