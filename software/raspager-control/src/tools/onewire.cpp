#include "onewire.h"

OneWire::OneWire() {
}

double OneWire::readTemp(string sSensor) {
	string Filename = "/sys/bus/w1/devices/";
	
	// Generate Filename for reading
	Filename.append(sSensor);
	Filename.append("/w1_slave");
	
	ifstream myfile (Filename);
	
	string line;
	if (myfile.is_open())
	{
		if (!getline (myfile,line)) {
			cout << "Reading from Sensor " << sSensor << "first line not possible" << endl;
			myfile.close();
			return -100;
		}
		
		std::regex line1rgx("([0-9a-f]{2} ){9}: crc=[0-9a-f]{2} YES");
		if (!std::regex_match(line, line1rgx)) {
			cout << "Reading from Sensor " << sSensor << "not correct" << endl;
			myfile.close();
			return -100;
		}
			
		if (!getline (myfile,line)) {
			cout << "Reading from Sensor " << sSensor << "second line not possible" << endl;
			myfile.close();
			return -100;
		}
			
		std::regex line2rgx("([0-9a-f]{2} ){9}t=([+-]?[0-9]+)");
		std::smatch matches;
		if (!std::regex_search(line, matches, line2rgx)) {
			cout << "Reading from Sensor " << sSensor << "second line not correct" << endl;
			myfile.close();
			return -100;
		} else	{
			// Temperature is in matches[2] in milli-degrees
			double TempRaw = stod(matches[2]);
		    myfile.close();
			return (TempRaw / 1000.0);
		}
	} else {
		cout << "Unable to open file " << Filename << endl;
		return -100;
	}
}

/*double OneWire::readTemp(string sSensor) {
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
*/