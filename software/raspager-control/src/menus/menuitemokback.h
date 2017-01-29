#pragma once

#include <vector>

#include "../tools/raspagerdigiextension.h"
#include "menuitem.h"

#define CURSOROKPLACE	18
#define CURSORBACKPLACE 21

class MenuItemOkBack : public MenuItem {
public:
    MenuItemOkBack ( string s, RaspagerDigiExtension* myExtension );

    void buttonUp();
    void buttonDown();
    bool stateOk;
    void setCursorOkBack();

    std::vector<MenuItem*> subMenuVector;
    std::vector<MenuItem*>::iterator myIterator;

    bool addSubmenuItem ( MenuItem* mySubMenuItem );
    bool hasSubmenu();

    virtual void printMenu ( bool reset = false );
    virtual int button ( int myTaster );
    virtual void process ( void ) {};
    virtual void processOK ( void );
};
