#pragma once

#include <stdint.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <bitset>





#define I2CADDRESS_ADC		0x51
#define I2CADDRESS_EXT4BIT	0x41
#define I2CADDRESS_EXT16BIT	0x24
#define I2CADDRESS_TMP		0x48
#define I2CADDRESS_DAC		0x60
#define TASTERBITPATTERN	0b01111100
#define TASTERUPPIN			(1<<2)
#define TASTERDOWNPIN		(1<<6)
#define TASTERLEFTPIN		(1<<5)
#define TASTERRIGHTPIN		(1<<3)
#define TASTEROKPIN			(1<<4)

#define TASTERNOTPRESSED 0
#define TASTERUP 1
#define TASTERDOWN 2
#define TASTERLEFT 3
#define TASTERRIGHT 4
#define TASTEROK 5

#define PCA9536OUTPUTREG	0x01
#define PCA9536CONFREG		0x03
#define PCA9536VSUPPLY		0x04
#define PCA9536ISUPPLY		0x06
#define PCA9536FWDPWR		0x02
#define PCA9536REVPWR		0x00
#define PCA9535ENABLEPWR	0x01



using namespace std;

class RaspagerDigiExtension {
private:
    int fdadc;
    int fddac;
    int fdExt4bit;
    int fdExt16bit;
    int fdtmp;
    int keypressed;
    int readADCValue();
    
    void lcdInit();
    void setLCDControlline ( int RS, int E );
    void setLCDDataLine ( int8_t data );
	void writeDACValue(unsigned int uiRawDACvalue);
	int readFwdPwrRaw();
	int readRevPwrRaw();
	int readCurrentRaw();
    int readVoltageRaw();
	void setMeasurementMultiplexer(char channel, char enablePWRDetector);
	
public:
    RaspagerDigiExtension(bool skipSetup);
    double readVoltage();
	double readCurrent();
	double readFwdPwr();
	double readRevPwr();
	double readSWR();

    int readTemperatureRaw();
    double readTemperature();
    void setOutputPower_Watt( double power_watt );
    int readButton();
    int readPortB();
    void testLCD();
    void lcdClear();
    void lcdCursorHome();
    void lcdWriteChar( char zeichen );
    void lcdWriteString( string mystring, int zeile = -1, int spalte = -1 );
    void lcdExecuteNormalCommand ( int8_t command );
    void lcdSetCursor( int zeile, int spalte );
    static string doubleValueToString(double val);
    static string shortUnsignedIntToString(short unsigned int val);
    static string boolToString(bool val);
};
