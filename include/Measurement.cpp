#include "Arduino.h"
#include "EEPROM_Lib.h"
#include "SHT31_Lib.h"
#include "Battery_Lib.h"
#include "Debug.h"
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

void performMeasurement(struct measurement *measureDatastore, uint16_t currentIteration, float currentCycleFactor) {
  measureDatastore->iterationMoment = currentIteration;
  measureTemperatureHumidity(measureDatastore);
  measureBattery(measureDatastore);
}

void writeMeasurementInEEPROM(struct measurement *measureDatastore) {
  // First read the counter
  uint16_t counter;
  readEEPROM(0, (byte*)&counter, sizeof(uint16_t));  
  // Write in the next available slot
  writeEEPROM( sizeof( uint16_t ) + counter++ * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
  // Update the counter
  writeEEPROM( 0, (byte*)&counter, sizeof(uint16_t));
}

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore) {
  readEEPROM(sizeof( uint16_t ) + measurementIndex * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
}

#ifdef DEBUG
void printMeasurementInSerial(struct measurement *measureDatastore) {
  char fConverter[10]; 
  
  dtostrf(getTemperature(measureDatastore), 0, 2, fConverter);
  Serial.println( "Temperature : "+String(fConverter)+" degC");

  dtostrf(getHumidity(measureDatastore), 0, 2, fConverter);
  Serial.println("Humidity : "+String(fConverter)+" %HR");  

  dtostrf(getVoltage(measureDatastore), 0, 3, fConverter);
  Serial.println("Battery voltage : "+String(fConverter)+" V");
}
#endif
