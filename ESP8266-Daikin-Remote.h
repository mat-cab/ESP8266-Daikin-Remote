#ifndef ESP8266_DAIKIN_REMOTE_H_
#define ESP8266_DAIKIN_REMOTE_H_

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

int32_t calculateCRC32(const uint8_t *data, size_t length);

int32_t getRTCmemCRC();

void writeRTCmem();

void writeEEPROM(uint16_t eeaddress, byte *data, uint8_t dataLength );

void readEEPROM(uint16_t eeaddress, byte* dataPointer, uint8_t dataLength);

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);

#endif
