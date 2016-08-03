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
/*        this->myRaspagerDigiExtension->lcdWriteString("Temperatur: " + this->myRaspagerDigiExtension->doubleValueToString(this->myRaspagerDigiExtension->readTemperature()), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("C", 1, 21);
*/
        this->myRaspagerDigiExtension->lcdWriteString("Spannung:   " + this->myRaspagerDigiExtension->doubleValueToString(this->myRaspagerDigiExtension->readVoltage()), 2, 1);
        this->myRaspagerDigiExtension->lcdWriteString("V", 2, 21);
        this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
    }
}
