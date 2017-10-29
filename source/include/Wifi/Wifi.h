#ifndef Wifi_Lib_H_
#define Wifi_Lib_H_

#include "Arduino.h"

#include "../Measurement.h"

// Wifi delay loop time
#define WIFI_WAIT 1000
// JSON Buffer size
#define JSON_BUFFER_MAX_LENGTH 1024

// Constants for the IOT stream
// Data server of the IOT Stream
#define IOT_SERVER "api.thingspeak.com"
// Port of the IOT Server
#define IOT_PORT 80
// Maximum number of points to be sent to the server
#define IOT_MAX_POINTS_PER_REQ  980

// Constants for the schedule server
// Data server of the schedule (use HTTPS)
#define GIST_SERVER "gist.githubusercontent.com"
// Port for HTTPS
#define GIST_PORT 443

void initializeWifi();

void resetWifi();

void connectToWifi();

void disconnectWifi();
 
void sendWifi();

void jsonUpdate(String *jsonBuf);

String jsonCreateEntry(struct measurement *measureDatastore);

uint16_t jsonGetEntryLength(struct measurement *measureDatastore);

void updateRTCTimestamp(struct measurement *measureDatastore);

void receiveWifi();
#endif
