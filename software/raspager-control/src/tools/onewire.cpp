#include "onewire.h"

OneWire::OneWire() {
}

double OneWire::readTemp(string sSensor) {
    FILE *fp;
    string sOut;
    char var[40];
    string sCommand;

    sCommand = "/usr/local/bin/owread /";
    sCommand += sSensor;
    sCommand += "/temperature";	
    const char *cstrCommand = sCommand.c_str();	

    fp = popen(cstrCommand, "r");

    while (fgets(var, sizeof(var), fp) != NULL) {
        sOut = sOut + var;
    }
    pclose(fp);

    // cut out Temp Value

    // Example output
    // 123456789ABC
    //      24.5625

    sOut = sOut.substr(5, sOut.length() - 1);
    double dOut = std::stod (sOut);
    return dOut;
}
