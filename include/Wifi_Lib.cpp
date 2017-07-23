#include "Arduino.h"

#include "ESP8266WiFi.h"
#include "WiFiClient.h"

#include "../libraries/Time/Time.h"
#include "CustomConstants.h"
#include "EEPROM_Lib.h"
#include "CycleConstants.h"
#include "Debug_Lib.h"
#include "RTCMem_Lib.h"
#include "Wifi_Lib.h"

WiFiClient client;

time_t *RTCtimestamp2;
float *cycleFactor2;

void initializeWifi() {
  // Power off Wifi
  WiFi.mode(WIFI_OFF);
}

void sendWifi() {
  char header[100];
  String jsonBuffer;
  bool retry = true;

  // Initialize the pointer from RTC memory
  RTCtimestamp2 = getRTCPointer_timestamp();
  cycleFactor2 = getRTCPointer_cycleFactor();
 
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
  time_t measurementTime = *RTCtimestamp2 + measureDatastore->iterationMoment * CYCLE_TIME / 1000;
  
  dtostrf(getTemperature(measureDatastore), 5, 2, sTemp);
  dtostrf(getHumidity(measureDatastore), 5, 2, sHumidity);
  dtostrf(getVoltage(measureDatastore), 5, 3, sVoltage);
  
  *buf += "{\"created_at\":\""+String(measurementTime)+"\",\"field1\":"+String(sTemp)+",\"field2\":"+String(sHumidity)+",\"field3\":"+String(sVoltage)+"}";
}

void updateTime(String timestamp) {
  time_t lastTimestamp = now();
  uint32_t currentMillis = millis();
  
  TimeElements newTimeTE;
  time_t newTime;

  // month conversion from String to int
  int month;
  String monthString = timestamp.substring(8,11);

  if ( monthString == "Jan" ) {
      month = 1;
  } else if ( monthString == "Feb") {
      month = 2;
  } else if ( monthString == "Mar") {
      month = 3;
  } else if ( monthString == "Apr") {
      month = 4;
  } else if ( monthString == "May") {
      month = 5;
  } else if ( monthString == "Jun") {
      month = 6;
  } else if ( monthString == "Jul") {
      month = 7;
  } else if ( monthString == "Aug") {
      month = 8;
  } else if ( monthString == "Sep") { 
      month = 9;
  } else if ( monthString == "Oct") {
      month = 10;
  } else if ( monthString == "Nov") {
      month = 11;
  } else if ( monthString == "Dec") {
      month = 12;
  } else {
    month = 0;
  }  

  // Compute the new time
  newTimeTE.Year = timestamp.substring(12,16).toInt()-1970;
  newTimeTE.Month = month;
  newTimeTE.Day = timestamp.substring(5,7).toInt();
  newTimeTE.Hour = timestamp.substring(17,19).toInt();
  newTimeTE.Minute = timestamp.substring(20,22).toInt();
  newTimeTE.Second = timestamp.substring(23,25).toInt();
  newTime = makeTime(newTimeTE);

  // Adjust with the time zone
  int32_t zoneShift = TIMEZONE.substring(1,3).toInt() * 3600 + TIMEZONE.substring(3,5).toInt() * 60;
  if ( TIMEZONE.substring(0,1).equals("-") ) {
    newTime -= zoneShift;
  } else {
    newTime += zoneShift;
  }
  
  // Set the new time
  setTime(newTime);

  // Compute shift between two dates
  int32_t shift =  newTime - lastTimestamp;

  // Adjust cycle factor
  if (*cycleFactor2 == 0.0) {
    *cycleFactor2 = CYCLE_FACTOR;
  } else {
    *cycleFactor2 -= (float)shift * 1000.0 / (float)(CYCLE_TIME * CYCLE_ITERATIONS);
  }  

  debug("Shift was: "+String(shift)+" seconds");
  debug("Adjusted cycle factor: " + String(*cycleFactor2)); 

  *RTCtimestamp2 = newTime - (uint32_t)((currentMillis - 1000)/1000);
}
