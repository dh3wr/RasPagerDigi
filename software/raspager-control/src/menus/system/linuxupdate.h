#pragma once

#include <cstdlib>
#include <iostream>

#include "../../tools/raspagerdigiextension.h"
#include "../menuitemokback.h"

class MenuLinuxUpdate : public MenuItemOkBack {
private:
    bool updateKernel;
public:
    MenuLinuxUpdate ( string s, bool updateKernel, RaspagerDigiExtension* myExtension );
    virtual void processOK ( void );
    virtual int button ( int myTaster );
    virtual void printMenu ( bool resetCursor = false );
};
