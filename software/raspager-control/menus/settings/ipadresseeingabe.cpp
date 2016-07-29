#include "ipadresseeingabe.h"

MenuIpAdresseEingabe::MenuIpAdresseEingabe(string s, NetworkControl* myNetworkControl, RaspagerDigiExtension* myExtension) : MenuIpEingabe(s, myExtension) {
    this->init(NetworkControl::readToArray(myNetworkControl->readCurrentIp()));
}

int MenuIpAdresseEingabe::buttonOk() {
    if (state == IPEINGABESTATEOK) {
        cout << "Sichere neue IP-Einstellungen..." << std::endl;

        // Alte Einstellungen laden
        std::ifstream ifs("/etc/network/interfaces");
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        cout << "[1/4] Aktuelle Einstellungen geladen." << std::endl;

        // alte IP durch neue IP ersetzen
        size_t start_pos = content.find(originalIp);
        if(start_pos == std::string::npos) {
            return false;
        }
        content.replace(start_pos, originalIp.length(), getIpString());
        cout << "[2/4] Neue Einstellungen erstellt." << std::endl;

        // Neue Einstellungen in Datei schreiben
        std::ofstream out("/etc/network/interfaces");
        out << content;
        out.close();
        cout << "[3/4] Neue Einstellungen gespeichert." << std::endl;

        cout << "[4/4] Setzen der neuen IP-Adresse." << std::endl;
        system(("ifconfig eth0 " + getIpString()).c_str());

        this->settingsSaved();
    } else if (state == IPEINGABESTATEBACK) {
        return MENURETURN;
    }
    return MENUSTAY;
}
