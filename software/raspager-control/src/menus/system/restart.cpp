#include "restart.h"

MenuRestart::MenuRestart(string s, RaspagerDigiExtension* myExtension) : MenuItemOkBack(s, myExtension) {
}

void MenuRestart::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int MenuRestart::button(int myTaster) {
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
    case TASTEROK:
        if (this->stateOk == true) {
            cout << "Fuehre Neustart durch..." << std::endl;
            this->myRaspagerDigiExtension->lcdClear();
            this->myRaspagerDigiExtension->lcdWriteString("Neustart...", 1, 1);
            this->myRaspagerDigiExtension->lcdWriteString("Bitte warten.", 2, 1);
            this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
            system("reboot");
        } else {
            return MENURETURN;
        }
        break;
    }
    return MENUSTAY;
}

void MenuRestart::printMenu(bool resetCursor) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();
        this->myRaspagerDigiExtension->lcdWriteString("Sind Sie sicher?", 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("                 OK Back", 2, 1);
        if (resetCursor) {
            this->stateOk = true;
        }
        this->setCursorOkBack();
    }
}
