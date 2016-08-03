#include "raspagerdigiextension.h"

RaspagerDigiExtension::RaspagerDigiExtension(bool skipSetup) {
    if (skipSetup) {
        cout << "WARNING: Skipping I2C Communication Setup!" << endl;
        return;
    }

    cout << "Setting up I2C Communication with extension boards..." << endl;
    system("gpio load i2c");
    wiringPiSetup() ;

	// Setup GPIO Pin 14 to High = 3.3 V to supply I2C Level shifter and isolator
	
	pinMode (14, OUTPUT);
	digitalWrite (14, HIGH);
	// Some time to let the supply voltage be stabilized
	delay(500);
	
	
    // ADC
    fdadc = wiringPiI2CSetup(I2CADDRESS_ADC);
    // Init ADC into auto run mode, 4 ksps
    wiringPiI2CWriteReg8(fdadc, 0x02, 0xE0);

    // DAC
    fddac = wiringPiI2CSetup(I2CADDRESS_DAC);
    
	// Init DAC into normal Mode
	// Set Command 0x04 = Write Volatile Configuration bits Command
	// Vref: 0x00 = Extern VRef
	// PD1:PD0 : 0x03 = If powered down, pull output with 640 kOhm to GND
	// G : 0x00 = Gain of 1
	
	unsigned char ucDACConfigValue = (0x04 << 5) | (0x03 << 1) | 0x00;
//	wiringPiI2CWrite(fddac, 0b10000000);
	wiringPiI2CWrite(fddac, ucDACConfigValue);

/*    // Temp
    fdtmp = wiringPiI2CSetup(I2CADDRESS_TMP);
    // Init Temp Sensor to 12 Bit resolution
    wiringPiI2CWriteReg16(fdtmp, 0x01, 0b01100000);
*/
    // 4 Bit I2C Extension
    fdExt4bit = wiringPiI2CSetup(I2CADDRESS_EXT4BIT);
    // Init 4 Bit Mux: All Ports to Output Mode
    wiringPiI2CWriteReg8(fdExt4bit, PCA9536CONFREG, 0x00);
	
	// Enable PWR Detector and set default channel VSupply
	setMeasurementMultiplexer(PCA9536VSUPPLY, 1);

    // 16 Bit I2C Extension
    fdExt16bit = wiringPiI2CSetup(I2CADDRESS_EXT16BIT);
    // Set all Pins on Port A to Output
    wiringPiI2CWriteReg8(fdExt16bit, 0x00, 0x00);
    // Set Pin 0 and 1 on Port B to Output, Pins 2 to 6 to Input
    wiringPiI2CWriteReg8(fdExt16bit, 0x01, TASTERBITPATTERN);
    // Set Input Polarity all Pins on Port B to same logic state
    wiringPiI2CWriteReg8(fdExt16bit, 0x03, 0x00);
    // Set Interrupt On Change Enbale Port B, Pins 2 to 6
    wiringPiI2CWriteReg8(fdExt16bit, 0x05, TASTERBITPATTERN);
    // Set Default Value Port B, Pins 2 to 6
    wiringPiI2CWriteReg8(fdExt16bit, 0x07, TASTERBITPATTERN);
    // Set Port B, Pins 2 to 6 are compared against Default Value
    wiringPiI2CWriteReg8(fdExt16bit, 0x09, TASTERBITPATTERN);
    // Set Port B, Pins 2 to 6 Pull Up resistor
    wiringPiI2CWriteReg8(fdExt16bit, 0x0D, TASTERBITPATTERN);
    // Read INTCAPB to clear any yet occured interrupt flag
    wiringPiI2CReadReg8(fdExt16bit, 0x11);
    lcdInit();

    keypressed = 0;
}


int RaspagerDigiExtension::readADCValue() {
    int myadcvalue = wiringPiI2CReadReg16(fdadc, 0x00);
    // swap lower and higher bit
    int myadcLowerByte = (myadcvalue & 0xFF00) >> 8;
    int myadcHigherByte = (myadcvalue & 0x00FF) << 8;

    return (myadcHigherByte | myadcLowerByte);
}

void RaspagerDigiExtension::setMeasurementMultiplexer(char channel, char enablePWRDetector) {
	wiringPiI2CWriteReg8(fdExt4bit, PCA9536OUTPUTREG, (channel | enablePWRDetector));
	// Wait some time to let the volages get stable
	delay(50);
}

int RaspagerDigiExtension::readVoltageRaw() {
    // Select Mux to Volatage input
    setMeasurementMultiplexer(PCA9536VSUPPLY, 1);
    // Read raw value from ADC
    return readADCValue();
}

int RaspagerDigiExtension::readFwdPwrRaw() {
	setMeasurementMultiplexer(PCA9536FWDPWR, 1);
	return readADCValue();
}

int RaspagerDigiExtension::readRevPwrRaw() {
	setMeasurementMultiplexer(PCA9536REVPWR, 1);
	return readADCValue();
}

int RaspagerDigiExtension::readCurrentRaw() {
	setMeasurementMultiplexer(PCA9536ISUPPLY, 1);	
	return readADCValue();
}


double RaspagerDigiExtension::readVoltage() {
    double res;
    res = ((double)readVoltageRaw() / 4096 * 5) / 10000 * (10000 + 33000);
    return res;
}

double RaspagerDigiExtension::readCurrent() {
    double res;
    res = ((double)readCurrentRaw() / 4096 * 5);
    return res;
}

double RaspagerDigiExtension::readFwdPwr() {
    double res;
    res = ((double)readFwdPwrRaw() / 4096 * 5);
    if (res < 0.35) {
	return 0;
    } else if (res > 2.5) {
	return 999;
    } else {
	double inputPwrMeter = 24.19354839 * (res - 0.5) + (-25);
	double PwrFwr = pow(10, ((inputPwrMeter + 30) / 10)) / 1000;
	return PwrFwr;
    }
}

double RaspagerDigiExtension::readRevPwr(){
    double res;
    res = ((double)readRevPwrRaw() / 4096 * 5);
    if (res < 0.35) {
	return 0;
    } else if (res > 2.5) {
	return 999;
    } else {
	double inputPwrMeter = 24.19354839 * (res - 0.5) + (-25);
	double PwrRev = pow(10, ((inputPwrMeter + 30) / 10)) / 1000;
	return PwrRev;
    }
}

double RaspagerDigiExtension::readSWR(){
    double res;
	res = 0;
    return res;
}

/*
int RaspagerDigiExtension::readTemperatureRaw() {
    return (wiringPiI2CReadReg16(fdtmp, 0x00));
}

double RaspagerDigiExtension::readTemperature() {
    int mytempvalue = readTemperatureRaw();
    // swap lower and higher bit
    int mytempLowerByte = (mytempvalue & 0xFF00) >> 8;
    int mytempHigherByte = (mytempvalue & 0x00FF) << 8;

    int res = (mytempHigherByte | mytempLowerByte);

    double tmp = (double)res / 256;
    return tmp;
}
*/

void RaspagerDigiExtension::writeDACValue(unsigned int uiRawDACvalue) {
	// Übergabeparameter auf 12 Bit begrenzen
	uiRawDACvalue &= 0x0FFF;
	unsigned char ucRawDACvalueUpper = uiRawDACvalue >> 8;

	unsigned char ucRawDACvalueLower = uiRawDACvalue & 0x00FF;
	
	// Schreiben, Upperbyte ist "Adresse", Lowerbyte ist Nutzdaten
//	cout << std::bitset<8>(ucRawDACvalueUpper) << " " << std::bitset<8>(ucRawDACvalueLower) << endl;
		
	wiringPiI2CWriteReg8(fddac, ucRawDACvalueUpper, ucRawDACvalueLower);
}

void RaspagerDigiExtension::writeDACVoltage(double dVoltage) {
	double dDACvalue =  (dVoltage / 5.0) * 4095.0;
        unsigned int uiDACvalue = (unsigned int)dDACvalue;
	writeDACValue(uiDACvalue);
}

void RaspagerDigiExtension::setOutputPower_Watt(double dPower_Watt) {
	writeDACVoltage(dPower_Watt);
}

int RaspagerDigiExtension::readButton() {
    int intflags;
    if (keypressed == 0) {
//    printf ("noch kein Taster gedrückt\n");
        intflags = wiringPiI2CReadReg8(fdExt16bit, 0x0F); // Read INTFB

        if (intflags == 0) {
            return 0;
        } else {
            keypressed = 1;
//      printf("Taster gedrückt Hex: %x, programmiere default-Konfig um\n", intflags);
            delay(10);
            int defaultvalue = TASTERBITPATTERN ^ intflags;
//      printf("Neue Defaultvalue Hex: %x\n", defaultvalue);

            wiringPiI2CWriteReg8(fdExt16bit, 0x07, defaultvalue); // Set Default Value Port B, Pins 2 to 6

            // Interrupt Flag löschen
            wiringPiI2CReadReg8(fdExt16bit, 0x11); // Read INTCAPB
            // Tastendruck melden
            if ((intflags & TASTERUPPIN) == TASTERUPPIN) {
                return (TASTERUP);
            } else if ((intflags & TASTERDOWNPIN) == TASTERDOWNPIN) {
                return (TASTERDOWN);
            } else if ((intflags & TASTERLEFTPIN) == TASTERLEFTPIN) {
                return (TASTERLEFT);
            } else if ((intflags & TASTERRIGHTPIN) == TASTERRIGHTPIN) {
                return (TASTERRIGHT);
            } else if ((intflags & TASTEROKPIN) == TASTEROKPIN) {
                return (TASTEROK);
            } else {
                return (TASTERNOTPRESSED);
            }
        }
    } else {
        // keypressed == 1
        intflags = wiringPiI2CReadReg8(fdExt16bit, 0x0F); // Read INTFB
        if (intflags != 0) {
//      printf("Taste wurde losgelassen\n");
            keypressed = 0;
            wiringPiI2CWriteReg8(fdExt16bit, 0x07, TASTERBITPATTERN); // Set Default Value Port B, Pins 2 to 6
            wiringPiI2CReadReg8(fdExt16bit, 0x11); // Read INTCAPB
        }
        return 0;
    }
}

int RaspagerDigiExtension::readPortB() {
    return (wiringPiI2CReadReg8(fdExt16bit, 0x13)); // Read GPIOB
}

void RaspagerDigiExtension::lcdExecuteNormalCommand(int8_t command) {
    setLCDDataLine(command); // 8 Datenbits, 2 Zeilen, 5x7 Font
    delayMicroseconds(1);
    setLCDControlline(0, 1);
    delayMicroseconds(1);
    setLCDControlline(0, 0);
    delayMicroseconds(40);
}

void RaspagerDigiExtension::lcdInit() {
    setLCDControlline(0, 0);
    delayMicroseconds(1);
    lcdExecuteNormalCommand(0b00111000); // 8 Datenbits, 2 Zeilen, 5x7 Font
    lcdExecuteNormalCommand(0b00001110); // Display ein, Cursor ein, Blinken aus
    lcdClear();
}

void RaspagerDigiExtension::lcdWriteChar(char zeichen) {
    setLCDControlline(1, 0);
    delayMicroseconds(1);
    setLCDDataLine(zeichen);
    delayMicroseconds(1);
    setLCDControlline(1, 1);
    delayMicroseconds(1);
    setLCDControlline(1, 0);
    delayMicroseconds(40);
}

void RaspagerDigiExtension::testLCD() {

}

void RaspagerDigiExtension::lcdSetCursor(int zeile, int spalte) {
    int adresse = spalte - 1 + (0x40 * (zeile - 1));
    lcdExecuteNormalCommand(0x80 | adresse);
}

void RaspagerDigiExtension::setLCDControlline(int RS, int E) {
    int value = 0;
    if (RS != 0) {
        value = 1;
    }
    if (E != 0) {
        value = value + 2;
    }
    wiringPiI2CWriteReg8(fdExt16bit, 0x13, value);
}

void RaspagerDigiExtension::setLCDDataLine(int8_t data) {
    wiringPiI2CWriteReg8(fdExt16bit, 0x12, data);
}

void RaspagerDigiExtension::lcdClear() {
    setLCDDataLine(0x01); // Clear Display
    setLCDControlline(0, 1);
    setLCDControlline(0, 0);
    delayMicroseconds(1700);
}

void RaspagerDigiExtension::lcdCursorHome() {
    setLCDDataLine(0b00000010); //Cursor Home
    setLCDControlline(0, 1);
    setLCDControlline(0, 0);
    delayMicroseconds(1700);
}

void RaspagerDigiExtension::lcdWriteString(string myString, int zeile, int spalte) {
    if ((zeile != -1) && (spalte != -1)) {
        lcdSetCursor(zeile, spalte);
    }
    int i = 0;
    while (myString[i] != '\0') {
        lcdWriteChar(myString[i]);
        i++;
    }
}

string RaspagerDigiExtension::doubleValueToString(double val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

string RaspagerDigiExtension::shortUnsignedIntToString(short unsigned int val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

string RaspagerDigiExtension::boolToString(bool val) {
    return val ? "Yes" : "No";
}