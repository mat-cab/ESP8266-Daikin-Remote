#ifndef ESP8266_DAIKIN_REMOTE_H_
#define ESP8266_DAIKIN_REMOTE_H_

#include "include/Measurement.h"

void measureBattery(struct measurement *measureDatastore);

void performMeasurement(struct measurement *measureDatastore, uint16_t currentIteration, float currentCycleFactor);

float getTemperature(struct measurement *measureDatastore);

float getHumidity(struct measurement *measureDatastore);

float getVoltage(struct measurement *measureDatastore);

void printMeasurementInSerial(struct measurement *measureDatastore);

void sendWifi();

boolean jsonAddRecord(String *jsonBuf);

void jsonUpdate(String *jsonBuf);

void jsonAddEntry(String* buf, struct measurement *measureDatastore);

void updateTime(String timestamp);
#endif
