#pragma once

#include "../tools/raspagerdigiextension.h"

#define MENUSTAY	1
#define MENURETURN	10

class MenuItem {
private:
    string menubezeichner;
public:
    RaspagerDigiExtension* myRaspagerDigiExtension;

    MenuItem ( string s, RaspagerDigiExtension* myExtension );

    string getMenubezeichner ( void );
    void setMenubezeichner ( string s );

    bool active;
    void setActive ( bool myState );

    virtual int button ( int myTaster ) { return -1; };
    virtual void printMenu ( bool reset = false ) {};
    virtual void processOK ( void ) {};
};
