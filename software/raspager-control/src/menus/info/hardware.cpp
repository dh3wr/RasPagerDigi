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
	double U_PA = this->myRaspagerDigiExtension->readVoltage();
	double I_PA = this->myRaspagerDigiExtension->readCurrent();
	double Pwr_Fwd = this->myRaspagerDigiExtension->readFwdPwr();
	double Pwr_Rev = this->myRaspagerDigiExtension->readRevPwr();
	double SWR = this->myRaspagerDigiExtension->readSWR();



        this->myRaspagerDigiExtension->lcdClear();

        this->myRaspagerDigiExtension->lcdWriteString("U PA: " + this->myRaspagerDigiExtension->doubleValueToString(U_PA,1), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("V", 1, 10);

        this->myRaspagerDigiExtension->lcdWriteString("I PA: " + this->myRaspagerDigiExtension->doubleValueToString(I_PA,1), 1, 14);
        this->myRaspagerDigiExtension->lcdWriteString("A", 1, 24);

        this->myRaspagerDigiExtension->lcdWriteString("F " + this->myRaspagerDigiExtension->doubleValueToString(Pwr_Fwd,1), 2, 1);
        this->myRaspagerDigiExtension->lcdWriteString("W", 2, 6);

        this->myRaspagerDigiExtension->lcdWriteString("R " + this->myRaspagerDigiExtension->doubleValueToString(Pwr_Rev,1), 2, 9);
        this->myRaspagerDigiExtension->lcdWriteString("W", 2, 14);

	if (SWR == -1) {
            this->myRaspagerDigiExtension->lcdWriteString("No SWR ", 2, 17);
	} else {
	 
   this->myRaspagerDigiExtension->lcdWriteString("SWR " + this->myRaspagerDigiExtension->doubleValueToString(SWR,1), 2, 16);
	}

        this->myRaspagerDigiExtension->lcdSetCursor(2, 30);

    }
}
