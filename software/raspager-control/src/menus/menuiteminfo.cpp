#include "menuiteminfo.h"

MenuItemInfo::MenuItemInfo(string s, string s2, RaspagerDigiExtension* myExtension) : MenuItemOkBack(s, myExtension) {
    this->s2 = s2;
}

void MenuItemInfo::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int MenuItemInfo::button(int myTaster) {
    return MENURETURN;
}

void MenuItemInfo::printMenu(bool resetCursor) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();
        this->myRaspagerDigiExtension->lcdWriteString(this->getMenubezeichner(), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString(s2, 2, 1);
		this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
        if (resetCursor) {
            this->stateOk = true;
        }
    }
}
