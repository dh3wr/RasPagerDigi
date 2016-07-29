#pragma once

#include <cstdlib>
#include <iostream>

#include "../../tools/raspagerdigiextension.h"
#include "../menuitemokback.h"

class InfoHardware : public MenuItemOkBack {
public:
    InfoHardware ( RaspagerDigiExtension* myExtension );
    virtual void processOK ( void );
    virtual int button ( int myTaster );
    virtual void printMenu ( bool resetCursor = false );
};
