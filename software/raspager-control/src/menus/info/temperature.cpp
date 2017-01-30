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

        this->myRaspagerDigiExtension->lcdWriteString("Einlass " + this->myRaspagerDigiExtension->doubleValueToString(this->OneWirePtr->readTemp(IN),1), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("C", 1, 13);

        this->myRaspagerDigiExtension->lcdWriteString("PA " + this->myRaspagerDigiExtension->doubleValueToString(this->OneWirePtr->readTemp(PA),1), 1, 15);
        this->myRaspagerDigiExtension->lcdWriteString("C", 1, 24);

        this->myRaspagerDigiExtension->lcdWriteString("Auslass " + this->myRaspagerDigiExtension->doubleValueToString(this->OneWirePtr->readTemp(OUT),1), 2, 1);
        this->myRaspagerDigiExtension->lcdWriteString("C", 2, 13);

        this->myRaspagerDigiExtension->lcdWriteString("2m " + this->myRaspagerDigiExtension->doubleValueToString(this->OneWirePtr->readTemp(PA),1), 2, 15);
        this->myRaspagerDigiExtension->lcdWriteString("C", 2, 24);





    }
}
