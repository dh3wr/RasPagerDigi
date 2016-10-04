#include "temperature.h"

InfoTemperature::InfoTemperature(RaspagerDigiExtension* myExtension, OneWire* myOneWire) : MenuItemOkBack("Temperaturen", myExtension) {

    OneWirePtr = myOneWire;

}

void InfoTemperature::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int InfoTemperature::button(int myTaster) {
    return MENURETURN;
}

void InfoTemperature::printMenu(bool resetCursor) {
    if (this->active) {

        this->myRaspagerDigiExtension->lcdClear();

        this->myRaspagerDigiExtension->lcdWriteString("Einlass: " + this->OneWirePtr->readTemp(IN), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("C", 1, 16);

    }
}
