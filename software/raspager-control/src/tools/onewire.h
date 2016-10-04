#pragma once

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

using namespace std;

class OneWire {
#define	OUT	"10.9855C3000800"
#define IN	"10.246C89020800"
#define APRS	"10.119EB5020800"
#define PA	"10.0345B5020800"

public:
    OneWire();
    double readTemp(string sSensor);
};

