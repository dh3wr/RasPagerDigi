#include "network.h"

InfoNetwork::InfoNetwork(NetworkControl* myNetworkControl, RaspagerDigiExtension* myExtension) : MenuItemOkBack("Netzwerk", myExtension) {
}

void InfoNetwork::processOK(void) {
    this->setActive(true);
    this->stage = 1;
    this->printMenu(true);
}

int InfoNetwork::button(int myTaster) {
    if (stage == 3) {
        stage = 0;
        return MENURETURN;
    }
    printMenu(false, stage);
    stage++;
    return MENUSTAY;
}

void InfoNetwork::printMenu(bool resetCursor, int myStage) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();
        if (myStage == 0) {
            this->myRaspagerDigiExtension->lcdWriteString("IP: " + myNetworkControl.readCurrentIp(), 1, 1);
            this->myRaspagerDigiExtension->lcdWriteString("GW: " + myNetworkControl.readCurrentGateway(), 2, 1);
        } else if (myStage == 1) {
            string sn = myNetworkControl.readCurrentSubnet();

            string empty = "";
            for (std::size_t i = 0; i < sn.length(); i++) {
                empty = empty + " ";
            }

            this->myRaspagerDigiExtension->lcdWriteString("SN:   " + sn, 1, 1);
            this->myRaspagerDigiExtension->lcdWriteString("CIDR: " + empty + "/" + RaspagerDigiExtension::shortUnsignedIntToString(myNetworkControl.readCurrentSubnetCIDR()), 2, 1);
        } else if (myStage == 2) {
            this->myRaspagerDigiExtension->lcdWriteString("DNS 1: " + myNetworkControl.readNameserver(1), 1, 1);
            this->myRaspagerDigiExtension->lcdWriteString("DNS 2: " + myNetworkControl.readNameserver(2), 2, 1);
        }
        this->myRaspagerDigiExtension->lcdSetCursor(2, 30);
    }
}
