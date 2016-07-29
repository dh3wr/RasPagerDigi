#include "ipeingabe.h"

MenuIpEingabe::MenuIpEingabe( string s, RaspagerDigiExtension* myExtension) : MenuItem(s, myExtension) {
}

void MenuIpEingabe::init(vector<int> ip) {
    aktuellesOktet = 3;
    myNumberHandler[0] = new NumberHandler(3, ip[3]);
    myNumberHandler[1] = new NumberHandler(3, ip[2]);
    myNumberHandler[2] = new NumberHandler(3, ip[1]);
    myNumberHandler[3] = new NumberHandler(3, ip[0]);
    state = IPEINGABESTATEIP;

    originalIp = getIpString();
}

void MenuIpEingabe::printMenu(bool resetCursor) {
    if (this->active) {
        this->myRaspagerDigiExtension->lcdClear();
        this->myRaspagerDigiExtension->lcdWriteString(this->getMenubezeichner(), 1, 1);
        this->myRaspagerDigiExtension->lcdWriteString(this->getIpLineString(), 2, 1);
    }

    if (resetCursor) {
        this->myRaspagerDigiExtension->lcdSetCursor(2, 1);
    } else {
        this->myRaspagerDigiExtension->lcdSetCursor(2, this->getCursorStelle());
    }
}

void MenuIpEingabe::processOK(void) {
    this->setActive(true);

    state = IPEINGABESTATEIP;
    aktuellesOktet = 3;
    myNumberHandler[aktuellesOktet]->setAktuelleStelle(HUNDERTER);

    this->printMenu(true);
}

void MenuIpEingabe::settingsSaved() {
    originalIp = getIpString();
    this->myRaspagerDigiExtension->lcdWriteString("Einstellungen gesichert!", 1, 1);
    usleep(3000 * 1000);
    printMenu(true);
}

int MenuIpEingabe::button(int myTaster) {
    switch (myTaster) {
    case TASTERRIGHT:
        this->buttonRight();
        this->printMenu(false);
        break;
    case TASTERLEFT:
        this->buttonLeft();
        this->printMenu(false);
        break;
    case TASTERUP:
        this->buttonUp();
        this->printMenu(false);
        break;
    case TASTERDOWN:
        this->buttonDown();
        this->printMenu(false);
        break;
    case TASTEROK:
        this->buttonOk();
        break;
    }
}

int MenuIpEingabe::getIP(int oktetNumber) {
    if ((oktetNumber >= 0) && (oktetNumber <= 3)) {
        return (myNumberHandler[oktetNumber]->getNumber());
    } else {
        cout << "IP-Eingabe: Fehler, Oktetnummer " << oktetNumber << "existiert nicht!\n";
    }
}

void MenuIpEingabe::setIP(int oktetNumber, int ip) {
    if ((oktetNumber >= 0) && (oktetNumber <= 3)) {
        myNumberHandler[oktetNumber]->setNumber(ip);
    } else {
        cout << "IP-Eingabe: Fehler, Oktetnummer " << oktetNumber << "existiert nicht!\n";
    }
}

void MenuIpEingabe::setIpAll(int ip3, int ip2, int ip1, int ip0) {
    myNumberHandler[0]->setNumber(ip0);
    myNumberHandler[1]->setNumber(ip1);
    myNumberHandler[2]->setNumber(ip2);
    myNumberHandler[3]->setNumber(ip3);
}

void MenuIpEingabe::buttonUp() {
    if (state == IPEINGABESTATEIP) {
        myNumberHandler[aktuellesOktet]->buttonUp();
        if (myNumberHandler[aktuellesOktet]->getNumber() >= 255) {
            myNumberHandler[aktuellesOktet]->setNumber(0);
        }
    }
}

void MenuIpEingabe::buttonDown() {
    if (state == IPEINGABESTATEIP) {
        myNumberHandler[aktuellesOktet]->buttonDown();
        if (myNumberHandler[aktuellesOktet]->getNumber() >= 255) {
            myNumberHandler[aktuellesOktet]->setNumber(0);
        }
    }
}

int MenuIpEingabe::buttonLeft() {
    switch (state) {
    case IPEINGABESTATEIP: {
        /*  cout << "Vorher  IPeing. Links . Aktuelles Oktet " << aktuellesOktet << " aktuelle Stelle ";
        cout << myNumberHandler[3]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[2]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[1]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[0]->GetAktuelleStelle() << " ";
        cout << "\n";
        */
        int leftsuccess = myNumberHandler[aktuellesOktet]->buttonLeft();
        if (leftsuccess == NUMBERHANDLERLEFT) {
            // We left the oktet to the left :-), so handle this
            if (aktuellesOktet < 3) {
                // if there are still oktets, go to the next one on the left
                aktuellesOktet++;
                myNumberHandler[aktuellesOktet]->setAktuelleStelle(EINER);
            } else {
                // Wrap around to BACK
                state = IPEINGABESTATEBACK;
            }
        }
        /*  cout << "Nachher IPeing. Links.  Aktuelles Oktet " << aktuellesOktet << " aktuelle Stelle ";
        cout << myNumberHandler[3]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[2]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[1]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[0]->GetAktuelleStelle() << " ";
        cout << "\n";
        */
        break;
    }
    case IPEINGABESTATEBACK: {
        // Left of BACK is OK, so change the state to OK
        state = IPEINGABESTATEOK;
        break;
    }
    case IPEINGABESTATEOK: {
        // Left of OK is th IP, oktet = 0, Stelle = EINER
        state = IPEINGABESTATEIP;
        aktuellesOktet = 0;
        myNumberHandler[aktuellesOktet]->setAktuelleStelle(EINER);
        break;
    }
    } //switch
}

int MenuIpEingabe::buttonRight() {
    switch (state) {
    case IPEINGABESTATEIP: {
        /*cout << "Vorher  IPeing. Rechts. Aktuelles Oktet " << aktuellesOktet << " aktuelle Stelle ";
        cout << myNumberHandler[3]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[2]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[1]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[0]->GetAktuelleStelle() << " ";
        cout << "\n";
        */

        int rightsuccess = myNumberHandler[aktuellesOktet]->buttonRight();
        if (rightsuccess == NUMBERHANDLERRIGHT) {
            // We left to the right, so handle this
            if (aktuellesOktet > 0) {
                // if there are still oktets, go to the next one on the right
                aktuellesOktet--;
                myNumberHandler[aktuellesOktet]->setAktuelleStelle(HUNDERTER);
            } else {
                // Go to OK STATE
                state = IPEINGABESTATEOK;
            }
        }

        /*  cout << "Nachher IPeing. Rechts. Aktuelles Oktet " << aktuellesOktet << " aktuelle Stelle ";
        cout << myNumberHandler[3]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[2]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[1]->GetAktuelleStelle() << " ";
        cout << myNumberHandler[0]->GetAktuelleStelle() << " ";
        cout << "\n";
        */
        break;
    }
    case IPEINGABESTATEBACK: {
        // right of BACK is nothing, so wrap around to IP, oktet = 3, Stelle = HUBDERTER
        state = IPEINGABESTATEIP;
        aktuellesOktet = 3;
        myNumberHandler[aktuellesOktet]->setAktuelleStelle(HUNDERTER);
        break;
    }
    case IPEINGABESTATEOK: {
        // right of OK is BACK
        state = IPEINGABESTATEBACK;
        break;
    }
    }
}

int MenuIpEingabe::getCursorStelle() {
    switch (state) {
    case IPEINGABESTATEIP:
        return (myNumberHandler[aktuellesOktet]->getAktuelleStelleCursor() + 4 * (3 - aktuellesOktet) + 1);
        break;
    case IPEINGABESTATEOK:
        return (18);
        break;
    case IPEINGABESTATEBACK:
        return (21);
        break;
    }
}

string MenuIpEingabe::getIpString() {
    char buffer[16];
    sprintf(buffer, "%d.%d.%d.%d", getIP(3), getIP(2), getIP(1), getIP(0));
    string s = (string)buffer;
    return (s);
}

string MenuIpEingabe::getIpLineString() {
    char buffer[25];
    sprintf(buffer, "%03d.%03d.%03d.%03d  OK Back", getIP(3), getIP(2), getIP(1), getIP(0));
    string s = (string)buffer;
    return (s);
}
