#include "menuitemokback.h"

MenuItemOkBack::MenuItemOkBack(string s, RaspagerDigiExtension* myExtension) : MenuItem(s, myExtension) {
}

bool MenuItemOkBack::addSubmenuItem(MenuItem* mySubMenuItem) {
    if (mySubMenuItem != NULL) {
        this->subMenuVector.push_back(mySubMenuItem);
        this->myIterator = this->subMenuVector.begin();
        return (true);
    } else {
        return (false);
    }
}

bool MenuItemOkBack::hasSubmenu() {
    return (not this->subMenuVector.empty());
}

void MenuItemOkBack::processOK(void) {
    this->myIterator = this->subMenuVector.begin();
    this->setActive(true);
    this->printMenu(true);
}

void MenuItemOkBack::printMenu(bool resetCursor) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();
        this->myRaspagerDigiExtension->lcdWriteString((*this->myIterator)->getMenubezeichner(), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("                 OK Back", 2, 1);
        if (resetCursor) {
            this->stateOk = true;
        }
        this->setCursorOkBack();
    } else {
        (*this->myIterator)->printMenu();
    }
}

void MenuItemOkBack::setCursorOkBack() {
    if (this->stateOk) {
        this->myRaspagerDigiExtension->lcdSetCursor(2, CURSOROKPLACE);
    } else {
        this->myRaspagerDigiExtension->lcdSetCursor(2, CURSORBACKPLACE);
    }
}

int MenuItemOkBack::button(int myTaster) {
    if (this->active) {
        switch (myTaster) {
        case TASTERRIGHT:
            if (this->stateOk == true) {
                this->stateOk = false;
                this->setCursorOkBack();
            }
            break;
        case TASTERLEFT:
            if (this->stateOk == false) {
                this->stateOk = true;
                this->setCursorOkBack();
            }
            break;
        case TASTERUP:
            this->buttonUp();
            break;
        case TASTERDOWN:
            this->buttonDown();
            break;
        case TASTEROK:
            if (this->stateOk == true) {
                this->setActive(false);
                (*this->myIterator)->setActive(true);
                (*this->myIterator)->processOK();
            } else {
                return MENURETURN;
            }
            break;
        }
    } else {
        if ((*this->myIterator)->button(myTaster) == MENURETURN) {
            (*this->myIterator)->setActive(false);
            this->setActive(true);
            this->printMenu();
        }
    }
    //return menuitemOkBack::Button(myTaster);
    return MENUSTAY;
}

void MenuItemOkBack::buttonDown() {
    if (this->myIterator == this->subMenuVector.end() - 1) {
//    cout << "ButtonDown ist am Ende\n";
        this->myIterator = this->subMenuVector.begin();
    } else {
        this->myIterator++;
    }
    this->printMenu(true);
}

void MenuItemOkBack::buttonUp() {
    if (this->myIterator == this->subMenuVector.begin()) {
//    cout << "ButtonUp ist am Anfang\n";
        this->myIterator = this->subMenuVector.end() - 1;
    } else {
        this->myIterator--;
    }
    this->printMenu(true);
//  cout << "Mainmenu ButtonUp " << this->myIterator - subMenuVector.begin()  << "\n";
}
