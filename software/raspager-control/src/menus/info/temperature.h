#pragma once

#include <cstdlib>
#include <iostream>

#include "../../tools/raspagerdigiextension.h"
#include "../menuitemokback.h"
#include "../../tools/onewire.h"

class InfoTemperature : public MenuItemOkBack {
public:
    InfoTemperature ( RaspagerDigiExtension* myExtension, OneWire* myOneWire );
    virtual void processOK ( void );
    virtual int button ( int myTaster );
    virtual void printMenu ( bool resetCursor = false );
private:
    OneWire* OneWirePtr;
};
