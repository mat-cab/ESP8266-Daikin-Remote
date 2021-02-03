#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "WiFiClientSecure.h"

#include "Time.h"

#include "../CustomConstants.h"
#include "../RTCMem_Lib.h"
#include "../ExPROM/ExPROM.h"
#include "../CycleManager/CycleManager.h"
#include "../Debug_Lib.h"
#include "../RTCMem_Lib.h"
#include "../Scheduler/Scheduler.h"
#include "Wifi.h"

WiFiClient * client;
WiFiClientSecure * sClient;

Wifi_RTCData * wifiRTCData;

void initializeWifi() {
  // Set the appropriate pointer
  wifiRTCData = getRTCPointer_wifiRTCData();  
}

void resetWifi() {
  debug("Resetting the wifi");

  // set the initial timestamp to 0
  wifiRTCData->setTimestamp(getCurrentCycleStart());

  debug("Reset done");
}

bool connectToWifi() {
  // Verify if already connected
  if (WiFi.status() != WL_CONNECTED) {
    // Activate Wifi
    WiFi.mode(WIFI_STA);

    // set initial number of retries
    uint8_t retries = WIFI_RETRIES;

    while (retries-->0) {
      int32_t timeout = WIFI_TIMEOUT;

      // Connect to Wifi
      WiFi.begin(WIFI_SSID, WIFI_PASSWD);

      // Output message
      debug("Trying to connect to Wifi...");

      // Wait until connected
      while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(WIFI_WAIT);
        timeout -= WIFI_WAIT;
        debug("Waiting for connection...");
      }

      if ( WiFi.status() == WL_CONNECTED ) {
        // Output message
        debug("Connected to Wifi!");
        // return OK
        return true;
      } else {
        // Say that timeout was reached
        debug("Timeout reached for Wifi connection");

        // If this was the last retry
        if (retries == 0 ) {
          // say that is was impossible to connect to the Wifi
          debug("Impossible to connect to specified Wifi.");
          // return error
          return false;
        }
      }
    }
  } else {
    // Say we are already connected
    debug("Wifi is already connected");

    // return OK
    return true;
  }
}

void disconnectWifi() {
  // Switch the WIFI off
  WiFi.mode(WIFI_OFF);
}

/***************************
 * Measurement send part
 ***************************/

void sendWifi() {
  char header[100];
  String jsonBuffer, nextEntry;
  bool connectionOK = false, retry = true;
  uint8_t retries = WIFI_SERVER_RETRIES;

  client = new WiFiClient();
 
  // Connect to server
  while (retry && retries-->0) {
    connectionOK = client->connect(IOT_SERVER, IOT_PORT);

    if ( !connectionOK ) {
      debug("Connection error with the IOT server");
      continue;
    }
    // Create the JSON buffer
    jsonBuffer = "{\"write_api_key\":\""+IOT_STREAM_KEY+"\",\"updates\":[";

    // Read the EEPROM counter
    uint16_t EEPROMcounter = readEEPROMMeasurementCounter();
    uint16_t totalMeasurements = EEPROMcounter < IOT_MAX_POINTS_PER_REQ ? EEPROMcounter : IOT_MAX_POINTS_PER_REQ;

    struct measurement recordedMeasure;

    // Compute the JSON string length (initial and final part of the string)
    uint32_t dataLength = jsonBuffer.length() + 3;

    // Initial value of the RTC Timestamp
    time_t RTCTimestamp = wifiRTCData->getTimestamp();

    // loop through all measurement
    for (uint16_t i=0; i<totalMeasurements; i++) {
      // Read a measurement from the EEPROM
      readMeasurementFromEEPROM(i, &recordedMeasure);
      // Add the length to the total length
      dataLength += jsonGetEntryLength(&recordedMeasure);
    }

    // Also add the , between the measures
    dataLength += totalMeasurements;
    
    // Output message
    debug("Sending the update request...");
    debug("JSON data is following (length is "+String(dataLength)+" bytes for "+totalMeasurements+" records)");

    delay(WIFI_WAIT);

    uint32_t writtenData = 0, totalWrittenData = 0, currentDataLength = 0;

    // Send the beginning of the request
    sprintf(header, "POST /channels/%s/bulk_update.json HTTP/1.1", IOT_CHANNEL_ID);
    client->println(header); 
    client->println("Host: api.thingspeak.com");
    client->println("User-Agent: mw.doc.bulk-update (Arduino ESP8266)");
    client->println("Connection: close");
    client->println("Content-Type: application/json");
    client->println("Content-Length: "+String(dataLength));
    client->println();

    // Loop through all the remaining entries
    for (uint16_t i=0; i<totalMeasurements; i++) {
      // Read a measurement from the EEPROM
      readMeasurementFromEEPROM(i, &recordedMeasure);

      // First increase the timestamp from the RTC memory (delta is wrt last measure)
      RTCTimestamp += recordedMeasure.deltaWithLastMeasurement;

      // Add the new entry to the JSON string
      nextEntry = jsonCreateEntry(&recordedMeasure, RTCTimestamp);

      if ( i != totalMeasurements - 1 ) {
        nextEntry += ",";
      }

      // Add what is possible to the next 
      currentDataLength = min( nextEntry.length(), JSON_BUFFER_MAX_LENGTH - jsonBuffer.length());
      jsonBuffer += nextEntry.substring(0, currentDataLength);

      // If current buffer is full
      if ( jsonBuffer.length() == JSON_BUFFER_MAX_LENGTH ) {
        // Send to debug
        debug(jsonBuffer);

        // Send the data to the server
        writtenData = client->print(jsonBuffer);

        if ( writtenData != jsonBuffer.length()) {
          debug("Error: sent data : "+String(jsonBuffer.length())+" - written data : "+writtenData);
        }

        totalWrittenData += writtenData;

        // reset the jsonBuffer to the rest of the next entry       
        jsonBuffer = nextEntry.substring( currentDataLength );
      }
    }

    // Write the end of the JSON data
    jsonBuffer += "]}";

    debug(jsonBuffer);

    writtenData = client->println(jsonBuffer);
            
    if ( writtenData != jsonBuffer.length() + 2) {
      debug("Error in last write: sent data : "+String(jsonBuffer.length()+2)+" - written data : "+writtenData);
    }

    totalWrittenData += writtenData;    

    debug("Total data written: "+String(totalWrittenData)+" out of "+dataLength+" expected");

    debug("Request sent!");

    // Wait for the client response
    while( !client->available() ) {
      // TODO: Implement a timeout for the request

      debug("Waiting for response...");
      
      delay(WIFI_WAIT);
    }

    debug("Response received!");

    int32_t timeShift = 0;

    // Output its response
    while (client->connected()) {
      if ( client->available() )
      {
        String str = client->readStringUntil('\n');
        
        if (str.startsWith("HTTP/1.1")) {
          // Do retry if return code is different from 202, which means OK
          retry = (str.substring(9,12) != "202");

          if (retry) {
            debug("Error reply was: "+str);
          }
        } else if (str.startsWith("Date: ")) {
          timeShift = updateTime( str.substring(6) );
        } else if (retry) {
          // There was an error, print the output message
          debug(str);
        }
      }     
    }

    if ( retry ) {
      // There was an error
      debug("Error while sending the request");
    } else {
      debug("Request sucessfully sent!");

      // Update the RTC timestamp
      wifiRTCData->setTimestamp(RTCTimestamp + timeShift);

      // Update the EEPROM
      deleteMeasurementsFromEEPROM( totalMeasurements ); 
    }
  }
}

String jsonCreateEntry(struct measurement *measureDatastore, time_t timestamp) {
  char sTemp[6], sHumidity[6], sVoltage[7], sFactor[8];

  String buf;

  uint16_t deltaTime = measureDatastore->deltaWithLastMeasurement;
 
  dtostrf(getTemperature(measureDatastore), 5, 2, sTemp);
  dtostrf(getHumidity(measureDatastore), 5, 2, sHumidity);
  dtostrf(getVoltage(measureDatastore), 5, 3, sVoltage);  

  char startTimestampChar[22];
  sprintf(startTimestampChar, "%04u-%02u-%02u %02u:%02u:%02u ", year(timestamp), month(timestamp), day(timestamp), hour(timestamp), minute(timestamp), second(timestamp));

  buf = "{\"created_at\":\""+String(startTimestampChar)+String(TIMEZONE)+"\",\"field1\":"+String(sTemp)+",\"field2\":"+String(sHumidity)+",\"field3\":"+String(sVoltage)+"}";
  
  return buf;
}

uint16_t jsonGetEntryLength(struct measurement *measureDatastore) {
  // Use the RTC timestamp as timestamp (this is just to get the length)
  return jsonCreateEntry(measureDatastore, wifiRTCData->getTimestamp()).length();
}

void updateRTCTimestamp(struct measurement *measureDatastore) {
  return wifiRTCData->increaseTimestamp( measureDatastore->deltaWithLastMeasurement );
}

/***************************
 * scheduler receive part
 ***************************/
bool receiveWifi(Action ** schedule) {
  char header[100];
  uint8_t serverRetries = WIFI_SERVER_RETRIES, pageRetries = WIFI_REQUEST_RETRIES;
  bool connectionOK;
  bool pageRetry;
  bool endOfHeader;

  sClient = new WiFiClientSecure();
  sClient->setInsecure();

  while (serverRetries-->0) {
    connectionOK = sClient->connect(GIST_SERVER, GIST_PORT);

    if ( !connectionOK ) {
      debug("Connection error with the schedule server");
    } else {
      pageRetry = true;

      while( pageRetry && pageRetries-->0 ) {
        endOfHeader = false;

        debug("Connected to schedule server");

        sprintf(header, "GET %s HTTP/1.1", SCHEDULE_URL);
        sClient->println(header); 
        sClient->println("Host: "+String(GIST_SERVER));
        sClient->println("Connection: close");
        sClient->println();

        // Here is the response
        while (sClient->connected()) {
          if ( sClient->available() ) {
            String str = sClient->readStringUntil('\n');
            
            if (endOfHeader == false) {
              if (str.startsWith("HTTP/1.1")) {
                // Do retry if return code is different from 200, which means OK
                pageRetry = (str.substring(9,12) != "200");

                if (pageRetry) {
                  debug("Error reply was: "+str);
                }
              } else if (str.startsWith("Date: ")) {
                  wifiRTCData->increaseTimestamp(updateTime( str.substring(6) ));
              } else if (str.length() == 1) {
                  endOfHeader = true;
                  
                  // Also reset the schedule to empty, before adding anything
                  *schedule = NULL;

                  debug("Here comes the schedule:");
              }
            } else {
              // We are in the schedule part!
              parseNewAction(str);
            }
          }     
        }
        
        if ( pageRetry ) {
          // There was an error
          debug("Error while receiving the schedule");
        } else {
          debug("Schedule sucessfully received!");
          // Do not reconnect to server
          serverRetries = 0;
        }
      }
    }

    // do not reconnect if all page retries are done
    if (pageRetries == 0) {
      serverRetries = 0;
    }
  }

  if (connectionOK && pageRetries > 0) {
    debug("Schedule updated !");
    return true;
  } else {
    if (!connectionOK) {
      debug("Could not reach schedule server");
    } else if( pageRetries == 0 ) {
      debug("Could not get schedule page");
      debug("Check the address of the schedule page ?");
    }
    return false;
  }
}
