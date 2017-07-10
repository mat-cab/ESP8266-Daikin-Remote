#ifndef SHT31_Lib_H_
#define SHT31_Lib_H_

// Constants for the SHT31 module
// SHT31 I2C Address
#define SHT31_ADDR 0x44
// Delay between SHT31 command to measure and data feedback
#define SHT31_DELAY 300

#include "Measurement.h"

// Function used to measure temperature and humidity
void measureTemperatureHumidity(struct measurement *measureDatastore);

#endif
