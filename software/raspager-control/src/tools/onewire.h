#pragma once

/* #include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <sstream>
 */
#include <iostream>
#include <fstream>
#include <string>
#include <regex>


using namespace std;

class OneWire {

#define	OUT		"10-000800c35598"
#define IN		"10-000802896c24"
#define APRS	"10-000802b59e11"
#define PA		"10-000802b54503"

#define TEMPEXT4	"10-000803164aa0"
#define TEMPEXT3	"10-000803160c3f"
#define TEMPEXT2	"10-000803168a4a"
#define TEMPEXT1	"10-000803162821"

public:
    OneWire();
    double readTemp(string sSensor);
};

