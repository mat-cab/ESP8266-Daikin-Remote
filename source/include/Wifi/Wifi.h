#ifndef Wifi_Lib_H_
#define Wifi_Lib_H_

#include "Arduino.h"

#include "../Measurement.h"
#include "../Scheduler/Action.h"

// Wifi delay loop time in ms
#define WIFI_WAIT 1000
// JSON Buffer size
#define JSON_BUFFER_MAX_LENGTH 1024

// Allowed retries for the WIFI connection
#define WIFI_RETRIES 3
// Allowed timeout for each WIFI connection in ms
#define WIFI_TIMEOUT 30000
// Allowed retries for the server connections
#define WIFI_SERVER_RETRIES 3
// Allowed retries for the requests
#define WIFI_REQUEST_RETRIES 10

// Constants for the IOT stream
// Data server of the IOT Stream
#define IOT_SERVER "api.thingspeak.com"
// Port of the IOT Server
#define IOT_PORT 80
// Maximum number of points to be sent to the server
#define IOT_MAX_POINTS_PER_REQ  960

// Constants for the schedule server
// Data server of the schedule (use HTTPS)
#define GIST_SERVER "gist.githubusercontent.com"
// Port for HTTPS
#define GIST_PORT 443

void initializeWifi();

void resetWifi();

bool connectToWifi();

void disconnectWifi();
 
void sendWifi();

void jsonUpdate(String *jsonBuf);

String jsonCreateEntry(struct measurement *measureDatastore, time_t timestamp);

uint16_t jsonGetEntryLength(struct measurement *measureDatastore);

void updateRTCTimestamp(struct measurement *measureDatastore);

bool receiveWifi(Action **schedule);
#endif
