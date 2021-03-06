#pragma once

#include <stdio.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "../../tools/networkcontrol.h"
#include "../../tools/numberhandler.h"
#include "../../tools/raspagerdigiextension.h"
#include "../menuitem.h"
#include "ipeingabe.h"

class MenuDnsEingabe : public MenuIpEingabe {
private:
    NetworkControl myNetworkControl;
public:
    MenuDnsEingabe ( string s, NetworkControl* myNetworkControl, int serverId, RaspagerDigiExtension* myExtension );
    int buttonOk();
};
