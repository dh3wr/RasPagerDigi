#include "menuiteminforotator.h"

MenuItemInfoRotator::MenuItemInfoRotator(string s, RaspagerDigiExtension* myExtension) : MenuItem(s, myExtension) {
    this->running = false;
}

bool MenuItemInfoRotator::addSubmenuItem(MenuItem* mySubMenuItem) {
    if (mySubMenuItem != NULL) {
        this->subMenuVector.push_back(mySubMenuItem);
        this->myIterator = this->subMenuVector.begin();
        return (true);
    } else {
        return (false);
    }
}

bool MenuItemInfoRotator::hasSubmenu() {
    return (not this->subMenuVector.empty());
}

void MenuItemInfoRotator::processOK(void) {
    this->myIterator = this->subMenuVector.begin();
}

/*
 * Läuft jeden SubMenu-Eintrag seiner selbst ab und lässt diesen sein Menü printen.
 * Wartet anschließend 5 Sekunden lang (blockierend!) und geht zum nächsten Eintrag über.
 */
void MenuItemInfoRotator::run() {
    this->running = true;

    this->setActive(true);
    for (std::vector<MenuItem*>::iterator it = this->subMenuVector.begin(); it != this->subMenuVector.end(); ++it) {
        (*it)->processOK();
        (*it)->setActive(false);
        usleep(5000 * 1000);
    }

    this->running = false;
}

bool MenuItemInfoRotator::isRunning(void) {
    return this->running;
}
