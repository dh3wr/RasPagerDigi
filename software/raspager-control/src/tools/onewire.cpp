#include "onewire.h"

OneWire::OneWire() {
}

string OneWire::readTemp(string sSensor) {
    FILE *fp;
    string out;
    char var[40];
    string sCommand;

    sCommand = "/usr/local/bin/owread /";
    sCommand += sSensor;
    sCommand += "/temperature";	
    const char *cstrCommand = sCommand.c_str();	

    fp = popen(cstrCommand, "r");

    while (fgets(var, sizeof(var), fp) != NULL) {
        out = out + var;
    }
    pclose(fp);

    // cut out Temp Value

    // Example output
    // 123456789ABC
    //      24.5625

    out = out.substr(5, out.length() - 1);
    return out;
}
