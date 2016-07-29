#include "menuitem.h"

MenuItem::MenuItem(string s, RaspagerDigiExtension* myExtension) {
    this->menubezeichner = s;
    this->myRaspagerDigiExtension = myExtension;
	this->active = false;

    cout << "MenuItem: " << s << std::endl;
}

string MenuItem::getMenubezeichner(void) {
    return this->menubezeichner;
}

void MenuItem::setMenubezeichner(string s) {
    this->menubezeichner = s;
}

void MenuItem::setActive(bool myState) {
    this->active = myState;
    if (myState) cout << "MenuItem: setActive(): " << menubezeichner << "\n";
}
