#include "linuxupdate.h"

MenuLinuxUpdate::MenuLinuxUpdate(string s, bool updateKernel, RaspagerDigiExtension* myExtension) : MenuItemOkBack(s, myExtension) {
    this->updateKernel = updateKernel;
}

void MenuLinuxUpdate::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int MenuLinuxUpdate::button(int myTaster) {
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
            cout << "Aktualisiere Linux-Pakete..." << std::endl;
            this->myRaspagerDigiExtension->lcdClear();
            this->myRaspagerDigiExtension->lcdWriteString("Aktualisiere...", 1, 1);
            this->myRaspagerDigiExtension->lcdWriteString("Bitte warten.", 2, 1);
            this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
            system("apt-get update && apt-get dist-upgrade -y && apt-get autoremove -y && apt-get clean");
            cout << "Vorgang beendet." << std::endl;

            if (this->updateKernel) {
                cout << "Aktualisiere Linux-Kernel..." << std::endl;
                this->myRaspagerDigiExtension->lcdClear();
                this->myRaspagerDigiExtension->lcdWriteString("Aktualisiere...", 1, 1);
                this->myRaspagerDigiExtension->lcdWriteString("Bitte warten.", 2, 1);
                this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
                system("rpi-update");
                cout << "Vorgang beendet. Bitte System neustarten." << std::endl;
            }
        }
        return MENURETURN;
        break;
    }
    return MENUSTAY;
}

void MenuLinuxUpdate::printMenu(bool resetCursor) {
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
