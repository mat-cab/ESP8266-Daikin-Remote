#ifndef I2C_Constants_H_
#define I2C_Constants_H_

// Constants for the ESP8266
// ESP8266 GPIO SDA pin for the I2C bus
#define ESP8266_SDA 12
// ESP8266 GPIO SCL pin for the I2C bus
#define ESP8266_SCL 14
// Max size for each I2C transfer in bytes
// NOTE: real max value is 32 bytes. However in I2C transfers the first two bytes are already taken for addresses
#define ESP8266_WIRE_BUFFER_LENGTH 30

#endif
