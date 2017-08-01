#ifndef DEBUG_H_
#define DEBUG_H_

// Used for debug purposes
// Comment for production
//#define DEBUG

#include "Arduino.h"
#include "Measurement.h"

void initializeDebug();

void endDebug();

void debug(String message);

void debug(struct measurement *measure);

#endif