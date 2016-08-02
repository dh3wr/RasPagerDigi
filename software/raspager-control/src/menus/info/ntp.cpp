#include "ntp.h"

InfoNtp::InfoNtp(NetworkControl* myNetworkControl, RaspagerDigiExtension* myExtension) : MenuItemOkBack("NTP-Status", myExtension) {
}

void InfoNtp::processOK(void) {
    this->setActive(true);
    this->printMenu(true);
}

int InfoNtp::button(int myTaster) {
    return MENURETURN;
}

void InfoNtp::printMenu(bool resetCursor) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();
        this->myRaspagerDigiExtension->lcdWriteString("NTP: " + myNetworkControl.readNtpServer(), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString("Synchronized: " + RaspagerDigiExtension::boolToString(myNetworkControl.NTPServerSynced()), 2, 1);
        this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
    }
}
