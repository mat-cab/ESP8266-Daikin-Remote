#include "Arduino.h"

#include "Wire.h"

#include "I2C_Lib.h"

void initializeI2C() {
  // Setup I2C connection
  Wire.begin(ESP8266_SDA,ESP8266_SCL);
  Wire.setClock(100000L);
}
