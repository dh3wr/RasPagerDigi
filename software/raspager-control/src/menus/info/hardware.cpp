#include "hardware.h"

InfoHardware::InfoHardware(RaspagerDigiExtension* myExtension) : MenuItemOkBack("Hardware-Info", myExtension) {
}

void InfoHardware::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int InfoHardware::button(int myTaster) {
    return MENURETURN;
}

void InfoHardware::printMenu(bool resetCursor) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();

        this->myRaspagerDigiExtension->lcdWriteString("U PA " + this->myRaspagerDigiExtension->doubleValueToString(this->myRaspagerDigiExtension->readVoltage()), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("V", 1, 11);

        this->myRaspagerDigiExtension->lcdWriteString("I PA  " + this->myRaspagerDigiExtension->doubleValueToString(this->myRaspagerDigiExtension->readCurrent()), 2, 1);
        this->myRaspagerDigiExtension->lcdWriteString("A", 2, 11);

        this->myRaspagerDigiExtension->lcdWriteString("P Fwd " + this->myRaspagerDigiExtension->doubleValueToString(this->myRaspagerDigiExtension->readFwdPwr()), 1, 14);
        this->myRaspagerDigiExtension->lcdWriteString("W", 1, 24);

        this->myRaspagerDigiExtension->lcdWriteString("P Rev " + this->myRaspagerDigiExtension->doubleValueToString(this->myRaspagerDigiExtension->readRevPwr()), 2, 14);
        this->myRaspagerDigiExtension->lcdWriteString("W", 2, 24);

        this->myRaspagerDigiExtension->lcdSetCursor(2, 30);

    }
}
