#pragma once

#include <math.h>

#include <iostream>

#define NUMBERHANDLEROK		1
#define NUMBERHANDLERLEFT	2
#define NUMBERHANDLERRIGHT	3
#define NUMBERHANDLERMAXSTELLEN	3
#define HUNDERTER		2
#define ZEHNER			1
#define EINER			0

using namespace std;

class NumberHandler {
private:
    int aktuelleStelle;
    int maxStellen;
    unsigned int stellen[NUMBERHANDLERMAXSTELLEN];

public:
    NumberHandler(int myMaxStellen, int initValue);
    void buttonUp();
    void buttonDown();
    int buttonLeft();
    int buttonRight();
    int getNumber();
    int getAktuelleStelle();
    int getAktuelleStelleCursor();
    void setAktuelleStelle(int myStelle);
    void setNumber(int initValue);
};
