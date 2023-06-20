#ifndef PINOUT
#define PINOUT
/* Garage Grade Engine Controller Pinout
Defines Pinout for Valves, RTDs, and 4-20 loops 
*/
#include "Arduino.h"
/*
// these track the digital pins wired up to the valve gate channels on the primary teensy
static int valve_cmd_map[] = {2, 3, 4, 5, 6, 7, 8, 9};

// these track the analog pins wired up to the current sense on the valve channels
static int vfb_map[] = {A13, A12, A11, A10, A14, A15, A16, A17};

// these track the analog pins wired up to 4_20 loops on the secondary teensy
static int SPI_4_20[] = {A6, A9, A8, A7, A2, A5, A4, A3, A16, A1, A0, A17};

// these track the analog pins wired up to the rtd channels on the primary teensy
static int rtd_map[] = {A5, A9, A4, A8, A3, A7, A2, A6};
*/
const int pinOut_LED = LED_BUILTIN;

const int pinOut_MS1 = 1;
const int pinOut_MS2 = 2;
const int pinOut_MS3 = 3; 

const byte pinOut_DIR = 33;
const byte pinOut_STEP = 34;

#endif
