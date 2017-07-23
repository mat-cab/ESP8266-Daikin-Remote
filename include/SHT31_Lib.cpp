#include "Arduino.h"

#include "Wire.h"

#include "SHT31_Lib.h"
#include "Measurement.h" 
#include "Debug_Lib.h"

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

  if( returnCode > 0 ) {
    debug("Error "+String(returnCode)+" while sending measurement command!");
  }

  // Wait for the data refresh
  delay(SHT31_DELAY);

  // Address SHT.31
  Wire.beginTransmission(SHT31_ADDR);
  // No command word
  returnCode = Wire.endTransmission(false);

  if( returnCode > 0 ) {
    debug("Error "+String(returnCode)+" while asking for measurement data!");
  }
  
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

    debug("Only "+String(returnCode)+" data bytes available");
    measureDatastore->temperature[0] = 0;
    measureDatastore->temperature[1] = 0;
    measureDatastore->humidity[0] = 0;
    measureDatastore->humidity[1] = 0;    
  }
}
