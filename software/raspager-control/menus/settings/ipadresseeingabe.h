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

class MenuIpAdresseEingabe : public MenuIpEingabe {
private:
    NetworkControl myNetworkControl;
public:
    MenuIpAdresseEingabe ( string s, NetworkControl* myNetworkControl, RaspagerDigiExtension* myExtension );
    int buttonOk();
};
