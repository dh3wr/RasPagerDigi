#include "systemcontrol.h"

SystemControl::SystemControl() {
}

string SystemControl::readKernelVersion() {
    FILE *fp,*outputfile;
    string out;
    char var[40];

    fp = popen("uname -s -r", "r");
    while (fgets(var, sizeof(var), fp) != NULL) {
        out = out + var;
    }
    pclose(fp);

    // cut off last char
    return out.substr(0, out.length() - 1);
}
