#pragma once

#include <unistd.h>

#include <vector>

#include "../tools/raspagerdigiextension.h"
#include "menuitem.h"
#include "menuitemokback.h"

class MenuItemInfoRotator : public MenuItem {
private:
    bool running;
public:
    MenuItemInfoRotator ( string s, RaspagerDigiExtension* myExtension );

    std::vector<MenuItem*> subMenuVector;
    std::vector<MenuItem*>::iterator myIterator;

    bool addSubmenuItem ( MenuItem* mySubMenuItem );
    bool hasSubmenu( void );

    void run ( void );
    bool isRunning ( void );
    void processOK ( void );
};
