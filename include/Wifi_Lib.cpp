#include "Arduino.h"

#include "ESP8266WiFi.h"
#include "WiFiClient.h"

#include "../libraries/Time/Time.h"
#include "CustomConstants.h"
#include "EEPROM_Lib.h"
#include "CycleManager_Lib.h"
#include "Debug_Lib.h"
#include "RTCMem_Lib.h"
#include "Wifi_Lib.h"

WiFiClient client;

void initializeWifi() {
  // Power off Wifi
  WiFi.mode(WIFI_OFF);
}

void sendWifi() {
  char header[100];
  String jsonBuffer;
  bool retry = true;
 
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

  // Connect to server
  while (client.connect(IOT_SERVER, IOT_PORT) && retry) {

    // Create the JSON buffer
    jsonBuffer = "{\"write_api_key\":\""+IOT_STREAM_KEY+"\",\"updates\":[";
    // Remember the length:
    uint16_t jsonBufferInitialLength = jsonBuffer.length();

    // Read the EEPROM counter
    uint16_t EEPROMcounter = 0;
    readEEPROM(0, (byte*)&EEPROMcounter, sizeof(uint16_t));
    struct measurement recordedMeasure;
    
    // Add the first record to the JSON string
    // Read the measurement from the EEPROM
    readMeasurementFromEEPROM(0, &recordedMeasure);
    // Add it to the buffer
    jsonAddEntry(&jsonBuffer, &recordedMeasure);

    // get the length of a single entry
    uint16_t jsonEntryLength = jsonBuffer.length() - jsonBufferInitialLength;

    // Compute the full expected JSON data length
    // Also add the comas between the records
    uint32_t dataLength = jsonBufferInitialLength + EEPROMcounter * jsonEntryLength + (EEPROMcounter-1) + 4;
    
    // Output message
    debug("Sending the update request...");
    debug("JSON data is following (length is "+String(dataLength)+" bytes for "+EEPROMcounter+" records)");

    delay(WIFI_WAIT);

    uint16_t writtenData = 0, totalWrittenData = 0;

    // Send the beginning of the request
    sprintf(header, "POST /channels/%s/bulk_update.json HTTP/1.1", IOT_CHANNEL_ID);
    client.println(header); 
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: mw.doc.bulk-update (Arduino ESP8266)");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+String(dataLength));
    client.println();

    // Loop through all the remaining entries
    for (uint16_t i=1; i<EEPROMcounter; i++) {
      jsonBuffer += ",";
      // If next entry does not fit current buffer, send actual buffer
      if ( jsonBuffer.length() + jsonEntryLength > JSON_BUFFER_MAX_LENGTH ) {
        // Send to debug
        debug(jsonBuffer);

        writtenData = client.print(jsonBuffer);

        if ( writtenData != jsonBuffer.length()) {
          debug("Error: sent data : "+String(jsonBuffer.length())+" - written data : "+writtenData);
        }

        totalWrittenData += writtenData;
        
        jsonBuffer = "";
      }
      
      // Read a measurement from the EEPROM
      readMeasurementFromEEPROM(i, &recordedMeasure);
      // Add the new entry to the JSON string
      jsonAddEntry(&jsonBuffer, &recordedMeasure);
    }

    // Write the end of the JSON data
    jsonBuffer += "]}";

    debug(jsonBuffer);

    writtenData = client.println(jsonBuffer);
            
    if ( writtenData != jsonBuffer.length() + 2) {
      debug("Error in last write: sent data : "+String(jsonBuffer.length()+2)+" - written data : "+writtenData);
    }

    totalWrittenData += writtenData;    

    debug("Total data written: "+String(totalWrittenData)+" out of "+dataLength+" expected");

    debug("Request sent!");

    // Wait for the client response
    while( !client.available() ) {
      debug("Waiting for response...");
      
      delay(WIFI_WAIT);
    }

    debug("Response received!");

    // Output its response
    while (client.connected()) {
      if ( client.available() )
      {
        String str = client.readStringUntil('\n');
        
        if (str.startsWith("HTTP/1.1")) {
          // Do retry if return code is different from 202, which means OK
          retry = (str.substring(9,12) != "202");

          if (retry) {
            debug("Error reply was: "+str);
          }
        } else if (str.startsWith("Date: ")) {
            updateTime( str.substring(6) );
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

  // Switch the WIFI off
  WiFi.mode(WIFI_OFF);
}

void jsonUpdate(String *jsonBuf) {
  // First part of the JSON string
  *jsonBuf = "{\"write_api_key\":\""+IOT_STREAM_KEY+"\", \"updates\":[";

  // Loop through all available measurements in the EEPROM
  uint16_t EEPROMcounter = 0;
  readEEPROM(0, (byte*)&EEPROMcounter, sizeof(uint16_t));
  struct measurement recordedMeasure;
  for (uint16_t i=0; i<EEPROMcounter; i++) {
    // If in between measures, add separator
    if ( i > 0 ) {
      *jsonBuf += ",";
    }
    // Read a measurement from the EEPROM
    readMeasurementFromEEPROM(i, &recordedMeasure);
    // Add the new entry to the JSON string
    jsonAddEntry(jsonBuf, &recordedMeasure);
  }

  // Add the finaf part of the JSON string
  *jsonBuf += "]}";
}

void jsonAddEntry(String* buf, struct measurement *measureDatastore) {
  char sTemp[6], sHumidity[6], sVoltage[7], sFactor[8];
  time_t measurementTime = getMeasurementTime( measureDatastore );
  
  dtostrf(getTemperature(measureDatastore), 5, 2, sTemp);
  dtostrf(getHumidity(measureDatastore), 5, 2, sHumidity);
  dtostrf(getVoltage(measureDatastore), 5, 3, sVoltage);
  
  *buf += "{\"created_at\":\""+String(measurementTime)+"\",\"field1\":"+String(sTemp)+",\"field2\":"+String(sHumidity)+",\"field3\":"+String(sVoltage)+"}";
}
