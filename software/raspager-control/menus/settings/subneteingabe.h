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

class MenuSubnetEingabe : public MenuIpEingabe {
private:
    NetworkControl myNetworkControl;
public:
    MenuSubnetEingabe ( string s, NetworkControl* myNetworkControl, RaspagerDigiExtension* myExtension );
    int buttonOk();
};
