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

//FIXME: Find appropriate place for RTCtimestamp
#include "../libraries/Time/Time.h"
extern time_t *RTCtimestamp;

void initializeWifi();
 
void passWifiVariables(float *cFactor, time_t *timestamp);

void sendWifi();

void jsonUpdate(String *jsonBuf);

void jsonAddEntry(String* buf, struct measurement *measureDatastore);

void updateTime(String timestamp);

#endif
