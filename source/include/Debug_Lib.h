#ifndef DEBUG_H_
#define DEBUG_H_

// Used for debug purposes
// Speed for the serial connection
#define DEBUG_SPEED 115200
// GPIO pin for activating/deactivating the DEBUG
#define DEBUG_PIN 4

#include "Arduino.h"
#include "Measurement.h"

void initializeDebug();

void endDebug();

void debug(String message);

void debug(struct measurement *measure);

#endif
