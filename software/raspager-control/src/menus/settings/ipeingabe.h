#pragma once

#include <stdio.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../tools/numberhandler.h"
#include "../../tools/raspagerdigiextension.h"
#include "../menuitem.h"

//xxx.xxx.xxx.xxx
//PI3.PI2.PI1.PI0

class MenuIpEingabe : public MenuItem {
private:
#define IPEINGABEMENUNOOP	0
#define IPEINGABEMENUOK		1
#define IPEINGABEMENUBACK	2

#define IPEINGABESTATEIP	1
#define IPEINGABESTATEOK	2
#define IPEINGABESTATEBACK	3

    int ip[4];
    int aktuellesOktet;
    NumberHandler* myNumberHandler[4];

public:
    MenuIpEingabe ( string s, RaspagerDigiExtension* myExtension );

    int state;
    string originalIp;

    virtual void processOK ( void );
    virtual int button ( int myTaster );
    virtual void printMenu ( bool resetCursor = false );

    void init(vector<int> ip);
    void settingsSaved();
    void setIpAll ( int ip3, int ip2, int ip1, int ip0 );
    int getIP ( int oktetNumber );
    void setIP ( int oktetNumber, int ip );
    void buttonUp();
    void buttonDown();
    int buttonLeft();
    int buttonRight();
    virtual int buttonOk() = 0;
    int getCursorStelle();
    string getIpString();
    string getIpLineString();
};
