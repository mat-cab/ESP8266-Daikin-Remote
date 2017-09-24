#ifndef Wifi_Lib_H_
#define Wifi_Lib_H_

#include "Measurement.h"

// Wifi delay loop time
#define WIFI_WAIT 1000
// JSON Buffer size
#define JSON_BUFFER_MAX_LENGTH 204

// Constants for the IOT stream
// Data server of the IOT Stream
#define IOT_SERVER "api.thingspeak.com"
// Port of the IOT Server
#define IOT_PORT 80

// Constants for the schedule server
// Data server of the schedule (use HTTPS)
#define GIST_SERVER "gist.githubusercontent.com"
// Port for HTTPS
#define GIST_PORT 443

void initializeWifi();

void connectToWifi();

void disconnectWifi();
 
void sendWifi();

void jsonUpdate(String *jsonBuf);

void jsonAddEntry(String* buf, struct measurement *measureDatastore);

void receiveWifi();
#endif
