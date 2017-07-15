// System includes
#include "ESP8266WiFi.h"
#include "Wire.h"
#include "Time.h"

// Project includes
#include "include/CustomConstants.h"
#include "include/I2C_Constants.h"
#include "include/Measurement.h"
#include "include/SHT31_Lib.h"
#include "include/RTCMem_Lib.h"
#include "include/EEPROM_Lib.h"
// Module include
#include "ESP8266-Daikin-Remote.h"

// Used for debug purposes
// Comment for production
//#define DEBUG

// Constants for the cycles
// Cycle time in ms
#define CYCLE_TIME 30000
#define CYCLE_ITERATIONS 60
#define CYCLE_FACTOR 1.02

// Wifi delay loop time
#define WIFI_WAIT 1000
// JSON Buffer size
#define JSON_BUFFER_MAX_LENGTH 204

// Constants for the battery check function
// GPIO to activate the battery check circuit
#define BATCHECK_ACT  13
// GPIO to read the battery voltage
#define BATCHECK_READ A0
// Delay between circuit activation and measure
#define BATCHECK_DELAY 10
// Number of measures (to compute the mean value)
#define BATCHECK_LOOPS  10
// Delay between each measures
#define BATCHECK_LOOP_DELAY 10

// Constants for the IOT stream
// Data server of the IOT Stream
#define IOT_SERVER "api.thingspeak.com"
// Port of the IOT Server
#define IOT_PORT 80

char message[128];

WiFiClient client;

float *cycleFactor;
time_t *RTCtimestamp;  

void setup() {
  // Power off Wifi
  WiFi.mode(WIFI_OFF);

  // Initial declaration
  uint16_t *iteration;
  struct rtcData *RTCdata;  

  // Read the whole RTC memory
  ESP.rtcUserMemoryRead(0, (uint32_t*) &rtcRawData, sizeof(rtcRawData));

  // Set the appropriate pointers
  RTCdata = (struct rtcData*)&(rtcRawData.data);
  RTCtimestamp = &(RTCdata->timestamp);
  iteration = &(RTCdata->iteration);

  // Verify the CRC
  boolean corruptedRTCmem = (rtcRawData.crc32 != getRTCmemCRC());
  if (corruptedRTCmem) {
    // if not consistent, consider last iteration to update time
    *iteration = CYCLE_ITERATIONS - 1;
  }

  // Set the estimated local time
  setTime( *RTCtimestamp + (time_t)(CYCLE_TIME*(*iteration + 1)/1000));  

#ifdef DEBUG
  // Setup serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266 starting - Time is "+String(weekday())+" "+String(day())+"/"+String(month())+"/"+String(year())+" "+String(hour())+":"+String(minute())+":"+String(second()));
  if (corruptedRTCmem) {
    Serial.println("RTC memory is corrupted");
  }
#endif

  // Set the voltage measurement pins appropriately
  pinMode(BATCHECK_READ, INPUT);
  pinMode(BATCHECK_ACT, OUTPUT);

  // Setup I2C connection
  Wire.begin(ESP8266_SDA,ESP8266_SCL);
  Wire.setClock(100000L);

  // Now that time is set, update other less usefull values
  measurement measure;  
  uint16_t EEPROMcounter = 0;  
  cycleFactor = &(RTCdata->cycle_factor);  

  // In case of reset, fix the initial values
  if (corruptedRTCmem) {  
      // also reset the cycleFactor to 0.0 (will be updated when time is updated)
      *cycleFactor = 0.0;
      // Set the EEPROM counter to 0
      writeEEPROM(0,(byte*)&EEPROMcounter,sizeof(uint16_t));
  }  
    
  // Pause before looping
  delay(100);

#ifdef DEBUG
  Serial.println("Performing measurements ...");
#endif

  performMeasurement(&measure, *iteration, *cycleFactor);

#ifdef DEBUG
  printMeasurementInSerial(&measure);

  Serial.println("Saving in EEPROM...");
#endif

  writeMeasurementInEEPROM(&measure);

  if ( *iteration == CYCLE_ITERATIONS - 1 ) {
#ifdef DEBUG
    readEEPROM(0, (byte*)&EEPROMcounter, sizeof(uint16_t));
    struct measurement recordedMeasure;
    Serial.println("Content of the EEPROM:");
    Serial.println("Counter: "+String(EEPROMcounter));
    for (uint16_t i=0; i<EEPROMcounter; i++) {
      readMeasurementFromEEPROM(i, &recordedMeasure);
      Serial.println("Measurement "+String(i)+" out of "+String(EEPROMcounter)+" :");
      printMeasurementInSerial(&recordedMeasure);
      Serial.println("--------------------");
    }
#endif
    
    // Send the data with Wifi
    sendWifi();

    // reset the counter in EEPROM to 0
    EEPROMcounter = 0;
    writeEEPROM(0,(byte*)&EEPROMcounter,sizeof(uint16_t));    
  }

#ifdef DEBUG
  Serial.println("*** End of iteration "+ String(*iteration) +" - Time is "+String(weekday())+" "+String(day())+"/"+String(month())+"/"+String(year())+" "+String(hour())+":"+String(minute())+":"+String(second())+" ***");

  Serial.flush();

  Serial.end();    
#endif

  // Compute remaining time (in case of cycle overflow)
  *iteration = *iteration + (1 + millis() / CYCLE_TIME);
  uint32_t waitMillis = CYCLE_TIME * (1 + millis() / CYCLE_TIME);
  uint32_t waitMicros = (waitMillis*1000-micros())*(*cycleFactor);

  // Compute new iteration
  *iteration =  *iteration % CYCLE_ITERATIONS;

  // Write the RTC memory data
  writeRTCmem();  
  
  // Go do deepsleep
  ESP.deepSleep(waitMicros);
}

void loop() {
  // Empty loop for the deepSleep
}

void measureBattery(struct measurement *measureDatastore) {
  //
  uint16_t ADCsum;
  ADCsum = 0;

  int ADCraw;
  
  // Activate Battery reader
  digitalWrite(BATCHECK_ACT, HIGH);
  
  // wait a litlle
  delay(BATCHECK_DELAY);

  // Loop to have a mean value
  for ( int ADCmeasures = 0; ADCmeasures < BATCHECK_LOOPS; ADCmeasures++ ) {
    // Read the ADC
    ADCsum += analogRead(BATCHECK_READ);

    // wait before next iteration
    delay(BATCHECK_LOOP_DELAY);
  }
  
  // Disable battery reader
  digitalWrite(BATCHECK_ACT, LOW);

  // Have a mean value
  measureDatastore->voltage = ADCsum / BATCHECK_LOOPS;
}

void performMeasurement(struct measurement *measureDatastore, uint16_t currentIteration, float currentCycleFactor) {
  measureDatastore->iterationMoment = currentIteration;
  measureTemperatureHumidity(measureDatastore);
  measureBattery(measureDatastore);
}

float getTemperature(struct measurement *measureDatastore) {
  return (-45.0 + ( 175.0 * ( (measureDatastore->temperature[0] * 256) + measureDatastore->temperature[1] ) / 65535.0 ));
}

float getHumidity(struct measurement *measureDatastore) {
  return ((100.0 * ( ( measureDatastore->humidity[0] * 256.0) + measureDatastore->humidity[1] ) ) / 65535.0);
}

float getVoltage(struct measurement *measureDatastore) {
  return (measureDatastore->voltage/ 531.0 * 3.314);
}

#ifdef DEBUG
void printMeasurementInSerial(struct measurement *measureDatastore) {
  char fConverter[10];
  
  dtostrf(getTemperature(measureDatastore), 0, 2, fConverter);
  sprintf(message,"Temperature : %s degC", fConverter);
  Serial.println(message);  

  dtostrf(getHumidity(measureDatastore), 0, 2, fConverter);
  sprintf(message,"Humidity : %s %%HR", fConverter);
  Serial.println(message);  

  dtostrf(getVoltage(measureDatastore), 0, 3, fConverter);
  sprintf(message,"Battery voltage : %s V", fConverter);
  Serial.println(message);
}
#endif

void sendWifi() {
  char header[100];
  String jsonBuffer;
  bool retry = true;
 
  // Activate Wifi
  WiFi.mode(WIFI_STA);

  // Connect to Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

#ifdef DEBUG
  // Output message
  Serial.println("Trying to connect to Wifi...");
#endif

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_WAIT);
#ifdef DEBUG
    Serial.println("Waiting for connection...");
#endif
  }

#ifdef DEBUG
  // Output message
  Serial.println("Connected to Wifi!");
#endif

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
    
#ifdef DEBUG
    // Output message
    Serial.println("Sending the update request...");
    Serial.println("JSON data is following (length is "+String(dataLength)+" bytes for "+EEPROMcounter+" records)");
#endif
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
#ifdef DEBUG
        Serial.println(jsonBuffer);
#endif
        writtenData = client.print(jsonBuffer);
#ifdef DEBUG        
        if ( writtenData != jsonBuffer.length()) {
          Serial.println("Error: sent data : "+String(jsonBuffer.length())+" - written data : "+writtenData);
        }
#endif
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
    writtenData = client.println(jsonBuffer);
#ifdef DEBUG
    Serial.println(jsonBuffer);
            
    if ( writtenData != jsonBuffer.length() + 2) {
      Serial.println("Error in last write: sent data : "+String(jsonBuffer.length()+2)+" - written data : "+writtenData);
    }
#endif
    totalWrittenData += writtenData;    
#ifdef DEBUG
    Serial.println("Total data written: "+String(totalWrittenData)+" out of "+dataLength+" expected");
#endif    

#ifdef DEBUG
    Serial.println("Request sent!");
#endif

    // Wait for the client response
    while( !client.available() ) {
#ifdef DEBUG      
      Serial.println("Waiting for response...");
#endif
      
      delay(WIFI_WAIT);
    }

#ifdef DEBUG
    Serial.println("Response received!");
#endif

    // Output its response
    while (client.connected()) {
      if ( client.available() )
      {
        String str = client.readStringUntil('\n');
        
        if (str.startsWith("HTTP/1.1")) {
          // Do retry if return code is different from 202, which means OK
          retry = (str.substring(9,12) != "202");

#ifdef DEBUG
          if (retry) {
            Serial.println("Error reply was: "+str);
          }
#endif
        } else if (str.startsWith("Date: ")) {
            updateTime( str.substring(6) );
        }
      }     
    }

#ifdef DEBUG
    if ( retry ) {
      // There was an error
      Serial.println("Error while sending the request");
    } else {
      Serial.println("Request sucessfully sent!");
    }
#endif
  }

  // Switch the WIFI off
  WiFi.mode(WIFI_OFF);
}

boolean jsonAddRecord(String *jsonBuf) {
  static uint16_t currentRecord;
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
  time_t measurementTime = *RTCtimestamp + measureDatastore->iterationMoment * CYCLE_TIME / 1000;
  
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
  if (*cycleFactor == 0.0) {
    *cycleFactor = CYCLE_FACTOR;
  } else {
    *cycleFactor -= (float)shift * 1000.0 / (float)(CYCLE_TIME * CYCLE_ITERATIONS);
  }  

#ifdef DEBUG
  Serial.println("Shift was: "+String(shift)+" seconds");
  Serial.println("Adjusted cycle factor: " + String(*cycleFactor));
#endif      

  *RTCtimestamp = newTime - (uint32_t)((currentMillis - 1000)/1000);
}
