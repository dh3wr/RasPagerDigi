#pragma once

#include <cstdlib>
#include <iostream>

#include "../tools/raspagerdigiextension.h"
#include "menuitemokback.h"

class MenuItemInfo : public MenuItemOkBack {
private:
    string s2;
public:
    MenuItemInfo ( string s, string s2, RaspagerDigiExtension* myExtension );
    virtual void processOK ( void );
    virtual int button ( int myTaster );
    virtual void printMenu ( bool resetCursor = false );
};
