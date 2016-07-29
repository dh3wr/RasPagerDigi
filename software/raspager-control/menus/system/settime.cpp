#include "settime.h"

MenuSetTime::MenuSetTime(string s, RaspagerDigiExtension* myExtension) : MenuItemOkBack(s, myExtension) {
}

void MenuSetTime::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int MenuSetTime::button(int myTaster) {
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
            cout << "Setze Zeit durch NTP-Server..." << std::endl;
            this->myRaspagerDigiExtension->lcdClear();
            this->myRaspagerDigiExtension->lcdWriteString("Synchronisiere Zeit...", 1, 1);
            this->myRaspagerDigiExtension->lcdWriteString("Bitte warten.", 2, 1);
            this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
            system("service ntp stop && ntpd -gq && service ntp start");
            cout << "Vorgang beendet." << std::endl;
        }
        return MENURETURN;
        break;
    }
    return MENUSTAY;
}

void MenuSetTime::printMenu(bool resetCursor) {
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
