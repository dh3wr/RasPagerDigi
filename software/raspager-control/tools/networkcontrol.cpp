#include "networkcontrol.h"

NetworkControl::NetworkControl() {
}

string NetworkControl::readCurrentIp() {
    FILE* lsofFile_p = popen("echo `ifconfig eth0 2>/dev/null|awk '/inet addr:/ {print $2}'|sed 's/addr://'`", "r");

    if (!lsofFile_p) {
        return "-1";
    }

    char buffer[4096];
    char* line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
    pclose(lsofFile_p);
//  cout << "Ausgabe ifconfig: " << buffer;
    string str = buffer;
    int s = str.size();
    std::string::iterator it;
    for (size_t x = 0; x < s ; x++) {
        if (str.at(x) == '\n') {
            it = str.begin() + x;
            str.erase(it);
            s--;							// Correction of string length
        }
    }
    return str;
}

string NetworkControl::readCurrentSubnet() {
    FILE* lsofFile_p = popen("echo `ifconfig eth0 2>/dev/null|awk '/Mask:/ {print $4}'|sed 's/Mask://'`", "r");

    if (!lsofFile_p) {
        return "-1";
    }

    char buffer[4096];
    char* line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
    pclose(lsofFile_p);
    string str = buffer;
    int s = str.size();
    std::string::iterator it;
    for (size_t x = 0; x < s ; x++) {
        if (str.at(x) == '\n') {
            it = str.begin() + x;
            str.erase(it);
            s--;							// Correction of string length
        }
    }
    return str;
}

string NetworkControl::readCurrentGateway() {
    FILE* lsofFile_p = popen("netstat -rn | grep 0.0.0.0 | awk '{print $2}' | grep -v \"0.0.0.0\"", "r");

    if (!lsofFile_p) {
        return "-1";
    }

    char buffer[4096];
    char* line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
    pclose(lsofFile_p);
//  cout << "Ausgabe ifconfig: " << buffer;
    string str = buffer;
    int s = str.size();
    std::string::iterator it;
    for (size_t x = 0; x < s ; x++) {
        if (str.at(x) == '\n') {
            it = str.begin() + x;
            str.erase(it);
            s--;							// Correction of string length
        }
    }
    return str;
}

short unsigned int NetworkControl::readCurrentSubnetCIDR() {
    string subnet = readCurrentSubnet();
    const char* charsubnet;
    charsubnet = subnet.c_str();
    return toCidr((char*) charsubnet);
}

string NetworkControl::readNameserver(unsigned short numberNameserver) {
    ifstream infile("/etc/resolv.conf");
    string line;
    string nameserverip = "";
    string pattern = "nameserver ";
    int nameservercount = 0;

    if (infile.is_open()) {
        while (getline(infile, line)) {
//      cout << line << endl;
            if (nameservercount < 2) {
                size_t found = line.find(pattern);
                if (found != string::npos) {
                    nameservercount++;
                    if (nameservercount == numberNameserver) {
                        nameserverip = line.substr(11);
                    }
                }
            }
        }
        infile.close();
        return nameserverip;
    } else {
        cout << "Unable to open file";
        return "";
    }
}

// FIXME [mehrere NTP-Server möglich; meist via Domain angegeben]
string NetworkControl::readNtpServer() {
    if (NTPServerSynced()) {
        FILE* lsofFile_p = popen("ntpstat | grep synchronised | egrep -o '[0-9]+.[0-9]+.[0-9]+.[0-9]+'", "r");
        if (!lsofFile_p) {
            return "-1";
        }

        char buffer[4096];
        char* line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
        pclose(lsofFile_p);
        string str = buffer;
        int s = str.size();
        std::string::iterator it;
        for (size_t x = 0; x < s ; x++) {
            if (str.at(x) == '\n') {
                it = str.begin() + x;
                str.erase(it);
                s--;							// Correction of string length
            }
        }
        return str;
    } else {
        return "";
    }
}

bool NetworkControl::NTPServerSynced() {
    string unsync = "unsynchronised";

    FILE* lsofFile_p = popen("ntpstat", "r");

    if (!lsofFile_p) {
        return -1;
    }

    char buffer[4096];
    char* line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
    pclose(lsofFile_p);
    string str = buffer;
    size_t found = str.find(unsync);
    return (found == string::npos);
}

unsigned short NetworkControl::toCidr(char* subnet) {
    unsigned short netmask_cidr;
    int ipbytes[4];

    netmask_cidr = 0;
    sscanf(subnet, "%d.%d.%d.%d", &ipbytes[0], &ipbytes[1], &ipbytes[2], &ipbytes[3]);

    for (int i = 0; i < 4; i++) {
        switch (ipbytes[i]) {
        case 0x80:
            netmask_cidr += 1;
            break;
        case 0xC0:
            netmask_cidr += 2;
            break;
        case 0xE0:
            netmask_cidr += 3;
            break;
        case 0xF0:
            netmask_cidr += 4;
            break;
        case 0xF8:
            netmask_cidr += 5;
            break;
        case 0xFC:
            netmask_cidr += 6;
            break;
        case 0xFE:
            netmask_cidr += 7;
            break;
        case 0xFF:
            netmask_cidr += 8;
            break;
        default:
            return netmask_cidr;
            break;
        }
    }
    return netmask_cidr;
}

vector<int> NetworkControl::readToArray(string input) {
    for (int i=0; i < input.length(); i++) {
        if (input[i] == '.')
            input[i] = ' ';
    }

    vector<int> arr;
    stringstream ss(input);
    int temp;
    while (ss >> temp) {
        arr.push_back(temp);
    }

    return arr;
}
