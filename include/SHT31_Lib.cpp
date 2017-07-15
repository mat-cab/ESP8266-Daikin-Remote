#include "Wire.h"

#include "SHT31_Lib.h"
#include "Measurement.h" 

void measureTemperatureHumidity(struct measurement *measureDatastore) {
  unsigned int data[6];
  int returnCode;
  
  // Address SHT31
  Wire.beginTransmission(SHT31_ADDR);
  // Ask for a new measure
  Wire.write(0x2C);
  Wire.write(0x06);
  // End transmission
  returnCode = Wire.endTransmission(false);

#ifdef DEBUG
  if( returnCode > 0 ) {
    sprintf(message, "Error %d while sending measurement command!", returnCode);
    Serial.println(message);
  }
#endif  

  // Wait for the data refresh
  delay(SHT31_DELAY);

  // Address SHT.31
  Wire.beginTransmission(SHT31_ADDR);
  // No command word
  returnCode = Wire.endTransmission(false);

#ifdef DEBUG
  if( returnCode > 0 ) {
    sprintf(message, "Error %d while asking for measurement data!", returnCode);
    Serial.println(message);
  }
#endif
  
  // Ask for data words from SHT31
  Wire.requestFrom((uint8_t)SHT31_ADDR, (uint8_t)6);
  // Check if data available
  returnCode = Wire.available();
  if (returnCode == 6) {
    // get temperature first
    measureDatastore->temperature[0] = Wire.read();
    measureDatastore->temperature[1] = Wire.read();
    // Drop the next byte (checksum ?)
    Wire.read();
    // get humidity then
    measureDatastore->humidity[0] = Wire.read();
    measureDatastore->humidity[1] = Wire.read();
    // Drop the next byte (checksum ?)
    Wire.read();
  } else {
#ifdef DEBUG    
    sprintf(message, "Only %d data bytes available", returnCode);
    Serial.println(message);
#endif
    measureDatastore->temperature[0] = 0;
    measureDatastore->temperature[1] = 0;
    measureDatastore->humidity[0] = 0;
    measureDatastore->humidity[1] = 0;    
  }
}

