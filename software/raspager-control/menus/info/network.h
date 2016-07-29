#pragma once

#include <cstdlib>
#include <iostream>

#include "../../tools/networkcontrol.h"
#include "../../tools/raspagerdigiextension.h"
#include "../menuitemokback.h"

class InfoNetwork : public MenuItemOkBack {
private:
    NetworkControl myNetworkControl;
    int stage;
public:
    InfoNetwork ( NetworkControl* myNetworkControl, RaspagerDigiExtension* myExtension );
    virtual void processOK ( void );
    virtual int button ( int myTaster );
    virtual void printMenu ( bool resetCursor = false, int stage = 0 );
};
