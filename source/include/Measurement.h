#ifndef Measurement_H_
#define Measurement_H_

#include "Arduino.h"

// structures for the meaasurement (stored in EEPROM)
struct measurement {
  uint32_t deltaWithLastMeasurement;
  byte humidity[2];
  byte temperature[2];
  uint16_t voltage;
};

float getTemperature(struct measurement *measureDatastore);

float getHumidity(struct measurement *measureDatastore);

float getVoltage(struct measurement *measureDatastore);

uint16_t getDelta(struct measurement *measureDatastore);

void performMeasurement(struct measurement *measureDatastore);
#endif
