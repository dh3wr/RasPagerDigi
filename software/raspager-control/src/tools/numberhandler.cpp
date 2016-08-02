#include "numberhandler.h"

NumberHandler::NumberHandler(int myMaxStellen, int initValue) {
    aktuelleStelle = HUNDERTER;
    maxStellen = myMaxStellen;
    for (int i = maxStellen - 1; i >= 0; i--) {
        stellen[i] = round((initValue - (initValue % (int) pow(10, i))) / (int) pow(10, i));
        initValue -= stellen[i] * (int) pow(10, i);
    }
}

void NumberHandler::buttonUp() {
    if (stellen[aktuelleStelle] >= 9) {
        stellen[aktuelleStelle] = 0;
    } else {
        stellen[aktuelleStelle]++;
    }
}

void NumberHandler::buttonDown() {
    if (stellen[aktuelleStelle] <= 0) {
        stellen[aktuelleStelle] = 9;
    } else {
        stellen[aktuelleStelle]--;
    }
}

int NumberHandler::buttonLeft() {
    if (aktuelleStelle >= maxStellen - 1) {
        return (NUMBERHANDLERLEFT);
    } else {
        aktuelleStelle++;
        return (NUMBERHANDLEROK);
    }
}

int NumberHandler::buttonRight() {
    if (aktuelleStelle <= 0) {
        return (NUMBERHANDLERRIGHT);
    } else {
        aktuelleStelle--;
        return (NUMBERHANDLEROK);
    }
}

int NumberHandler::getNumber() {
    int Number = 0;
    for (int i = 0; i < maxStellen; i++) {
        Number += round(stellen[i] * pow(10, i));
    }
    return (Number);
}

void NumberHandler::setNumber(int initvalue) {
    for (int i = maxStellen - 1; i >= 0; i--) {
        stellen[i] = round((initvalue - (initvalue % (int) pow(10, i))) / (int) pow(10, i));
        initvalue -= stellen[i] * (int) pow(10, i);
    }
}

int NumberHandler::getAktuelleStelle() {
    return (aktuelleStelle);
}

int NumberHandler::getAktuelleStelleCursor() {
    return (maxStellen - aktuelleStelle - 1);
}

void NumberHandler::setAktuelleStelle(int myStelle) {
    aktuelleStelle = myStelle;
}
