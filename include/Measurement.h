#ifndef Measurement_H_
#define Measurement_H_

#include "Arduino.h"

// structures for the meaasurement (stored in EEPROM)
struct measurement {
  uint16_t iterationMoment;
  byte humidity[2];
  byte temperature[2];
  uint16_t voltage;
};

float getTemperature(struct measurement *measureDatastore);

float getHumidity(struct measurement *measureDatastore);

float getVoltage(struct measurement *measureDatastore);

void performMeasurement(struct measurement *measureDatastore, uint16_t currentIteration, float currentCycleFactor);

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);

#endif
