#ifndef Battery_Lib_H_
#define Battery_Lib_H_

#include "Measurement.h"

// Constants for the battery check function
// GPIO to activate the battery check circuit
#define BATCHECK_ACT  13
// GPIO to read the battery voltage
#define BATCHECK_READ A0
// Delay between circuit activation and measure
#define BATCHECK_DELAY 50
// Number of measures (to compute the mean value)
#define BATCHECK_LOOPS  10
// Delay between each measures
#define BATCHECK_LOOP_DELAY 10

void initializeBatteryMeasurement();

void measureBattery(struct measurement *measureDatastore);

#endif
