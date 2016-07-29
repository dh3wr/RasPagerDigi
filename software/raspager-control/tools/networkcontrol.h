#pragma once

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class NetworkControl {
private:
    unsigned short toCidr(char* subnet);

public:
    NetworkControl();
    string readCurrentIp();
    string readCurrentSubnet();
    unsigned short readCurrentSubnetCIDR();
    string readCurrentGateway();
    string readNameserver(unsigned short numberNameserver);
    string readNtpServer();
    bool NTPServerSynced();
    static vector<int> readToArray(string input);
};
