#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include "adf7012.h"

// PIN Definitions:
    
#define LE 0
#define CE 7
#define CLK 3
#define SDATA 2
//#define TXDATA 11	//Revision 1
#define MUXOUT 13
#define TXCLK 14
#define CLKOUT 12
//#define ATCLK 16	//Revision 1
//#define ATDATA 15	//Revision 1
//#define HANDSHAKE 5	//Revision 1
//#define PTT 4		//Revision 1

//Revision 2:
#define ATCLK 11
#define ATDATA 10
#define HANDSHAKE 5
#define PTT 4


// Radio Parameters
#define CRYSTALFREQ 4915200UL
#define FREQUENCY 439987500UL     

#define ADF7012_CRYSTAL_DIVIDER 1 

// POCSAG Protokoll
#define POCSAG_PREAMBLE_CODEWORD 0xAAAAAAAA
#define POCSAG_IDLE_CODEWORD 0x7A89C197
#define POCSAG_SYNCH_CODEWORD 0x7CD215D8



extern "C" RadioAdf7012 * setup() {
  
  wiringPiSetup();
  pinMode(LE, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(SDATA, OUTPUT);
  pinMode(TXDATA, OUTPUT);
  pinMode(CLKOUT, OUTPUT);
  pinMode(MUXOUT, INPUT);
  pinMode(TXCLK, INPUT);
  pinMode(ATDATA, OUTPUT);
  pinMode(ATCLK, OUTPUT);
  pinMode(HANDSHAKE, INPUT);
  pinMode(PTT, INPUT);

  digitalWrite(ATDATA, LOW);
  digitalWrite(ATCLK, LOW);
  RadioAdf7012* radio = new RadioAdf7012();
  (*radio).setup();
  return radio;
}

extern "C" bool pttOn(RadioAdf7012 * radio) {

  int counter = 0;
  bool status = false;

  do {
    status = radio->ptt_on();
  } while(!status && counter++ <10);  
  return status;
}

extern "C" void pttOff(RadioAdf7012 * radio) {
 
  while(digitalRead(PTT) == 1) {
    delay(100);
  }

  digitalWrite(ATDATA, LOW);
  digitalWrite(ATCLK, LOW);

  radio->ptt_off();
}

// Changes from Ralf Wilke 31.10.16: Was unsigned long, now (signed) long
extern "C" bool sendByte(long data) {

  for(int c = 31; c>=0;c--){

    while(digitalRead(HANDSHAKE) == 0) {
      delayMicroseconds(100);
    }

    if ((data & (unsigned long)(1 << c)) >> c)
	digitalWrite(ATDATA, HIGH);
    else
	digitalWrite(ATDATA, LOW);

    delayMicroseconds(20);
    digitalWrite(ATCLK, HIGH);
    delayMicroseconds(100);
    digitalWrite(ATCLK, LOW);
    delayMicroseconds(50);
  }
}