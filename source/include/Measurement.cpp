#include "Arduino.h"
#include "SHT31_Lib.h"
#include "Battery_Lib.h"
#include "Measurement.h"

float getTemperature(struct measurement *measureDatastore) {
  return (-45.0 + ( 175.0 * ( (measureDatastore->temperature[0] * 256) + measureDatastore->temperature[1] ) / 65535.0 ));
}

float getHumidity(struct measurement *measureDatastore) {
  return ((100.0 * ( ( measureDatastore->humidity[0] * 256.0) + measureDatastore->humidity[1] ) ) / 65535.0);
}

float getVoltage(struct measurement *measureDatastore) {
  return (measureDatastore->voltage/ 531.0 * 3.314);
}

void performMeasurement(struct measurement *measureDatastore, uint16_t currentIteration) {
  measureDatastore->iterationMoment = currentIteration;
  measureTemperatureHumidity(measureDatastore);
  measureBattery(measureDatastore);
}
