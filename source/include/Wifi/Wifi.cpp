#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "WiFiClientSecure.h"

#include "Time.h"

#include "../CustomConstants.h"
#include "../RTCMem_Lib.h"
#include "../EEPROM_Data_Lib.h"
#include "../CycleManager/CycleManager.h"
#include "../Debug_Lib.h"
#include "../RTCMem_Lib.h"
#include "../Scheduler/Scheduler.h"
#include "Wifi.h"

WiFiClient * client;

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

void connectToWifi() {
  // Verify if already connected
  if (WiFi.status() != WL_CONNECTED) {
    // Activate Wifi
    WiFi.mode(WIFI_STA);

    // Connect to Wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);

    // Output message
    debug("Trying to connect to Wifi...");

    // Wait until connected
    while (WiFi.status() != WL_CONNECTED) {
      delay(WIFI_WAIT);
      debug("Waiting for connection...");
    }

    // Output message
    debug("Connected to Wifi!");
  } else {
    // Say we are already connected
    debug("Wifi is already connected");
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
  bool retry = true;

  client = new WiFiClient();
 
  // Connect to server
  while (client->connect(IOT_SERVER, IOT_PORT) && retry) {

    // Create the JSON buffer
    jsonBuffer = "{\"write_api_key\":\""+IOT_STREAM_KEY+"\",\"updates\":[";

    // Read the EEPROM counter
    uint16_t EEPROMcounter = readEEPROMMeasurementCounter();

    struct measurement recordedMeasure;


    // Compute the JSON string length (initial and final part of the string)
    uint32_t dataLength = jsonBuffer.length() + 3;

    // loop through all measurement
    for (uint16_t i=0; i<EEPROMcounter; i++) {
      // Read a measurement from the EEPROM
      readMeasurementFromEEPROM(i, &recordedMeasure);
      // Add the length to the total length
      dataLength += jsonGetEntryLength(&recordedMeasure);
    }

    // Also add the , between the measures
    dataLength += EEPROMcounter;
    
    // Output message
    debug("Sending the update request...");
    debug("JSON data is following (length is "+String(dataLength)+" bytes for "+(EEPROMcounter+1)+" records)");

    delay(WIFI_WAIT);

    uint16_t writtenData = 0, totalWrittenData = 0, currentDataLength = 0;

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
    for (uint16_t i=0; i<EEPROMcounter; i++) {
      // Read a measurement from the EEPROM
      readMeasurementFromEEPROM(i, &recordedMeasure);

      // First increase the timestamp in the RTC memory (delta is wrt last measure)
      updateRTCTimestamp(&recordedMeasure);

      // Add the new entry to the JSON string
      nextEntry = jsonCreateEntry(&recordedMeasure);

      if ( i != EEPROMcounter - 1 ) {
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
      debug("Waiting for response...");
      
      delay(WIFI_WAIT);
    }

    debug("Response received!");

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
          updateTime( str.substring(6) );
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
    }
  }
}

String jsonCreateEntry(struct measurement *measureDatastore) {
  char sTemp[6], sHumidity[6], sVoltage[7], sFactor[8];

  String buf;

  uint16_t deltaTime = measureDatastore->deltaWithLastMeasurement;
 
  dtostrf(getTemperature(measureDatastore), 5, 2, sTemp);
  dtostrf(getHumidity(measureDatastore), 5, 2, sHumidity);
  dtostrf(getVoltage(measureDatastore), 5, 3, sVoltage);  

  time_t startTimestamp = wifiRTCData->getTimestamp();

  char startTimestampChar[22];
  sprintf(startTimestampChar, "%04u-%02u-%02u %02u:%02u:%02u ", year(startTimestamp), month(startTimestamp), day(startTimestamp), hour(startTimestamp), minute(startTimestamp), second(startTimestamp));

  buf = "{\"created_at\":\""+String(startTimestampChar)+String(TIMEZONE)+"\",\"field1\":"+String(sTemp)+",\"field2\":"+String(sHumidity)+",\"field3\":"+String(sVoltage)+"}";
  
  return buf;
}

uint16_t jsonGetEntryLength(struct measurement *measureDatastore) {
  return jsonCreateEntry(measureDatastore).length();
}

void updateRTCTimestamp(struct measurement *measureDatastore) {
  return wifiRTCData->increaseTimestamp( measureDatastore->deltaWithLastMeasurement );
}

/***************************
 * scheduler receive part
 ***************************/
void receiveWifi() {
  char header[100];
  bool retry = true;
  bool endOfHeader;

  client = new WiFiClientSecure();

  while( client->connect(GIST_SERVER, GIST_PORT) && retry ) {
    endOfHeader = false;

    debug("Connected to schedule server");

    sprintf(header, "GET %s HTTP/1.1", SCHEDULE_URL);
    client->println(header); 
    client->println("Host: "+String(GIST_SERVER));
    client->println("Connection: close");
    client->println();

    // TODO: Clear the schedule only when it is sure we will receive the new one

    // Here is the response
    while (client->connected()) {
      if ( client->available() ) {
        String str = client->readStringUntil('\n');
        
        if (endOfHeader == false) {
          if (str.startsWith("HTTP/1.1")) {
            // Do retry if return code is different from 200, which means OK
            retry = (str.substring(9,12) != "200");

            if (retry) {
              debug("Error reply was: "+str);
            }
          } else if (str.startsWith("Date: ")) {
              updateTime( str.substring(6) );
          } else if (str.length() == 1) {
              endOfHeader = true;
              debug("Here comes the schedule:");
          }
        } else {
          // We are in the schedule part!
          parseNewAction(str);
        }
      }     
    }
    
    if ( retry ) {
      // There was an error
      debug("Error while receiving the schedule");
    } else {
      debug("Schedule sucessfully received!");
    }
  }

  debug("Schedule updated !");
}
