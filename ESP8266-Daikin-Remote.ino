// System includes
#include "libraries/Time/Time.h"

// Project includes
#include "include/CustomConstants.h"
#include "include/I2C_Lib.h"
#include "include/Measurement.h"
#include "include/SHT31_Lib.h"
#include "include/RTCMem_Lib.h"
#include "include/EEPROM_Lib.h"
#include "include/Battery_Lib.h"
#include "include/Wifi_Lib.h"
#include "include/CycleConstants.h"

// Used for debug purposes
// Comment for production
//#define DEBUG

char message[128];

float *cycleFactor;
time_t *RTCtimestamp;  

void setup() {
  // Save the power
  initializeWifi();

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

  // Init the pins for the battery measurement
  initializeBatteryMeasurement();

  // Init the I2C protocol
  initializeI2C();

  // Now that time is set, update other less usefull values
  measurement measure;  
  uint16_t EEPROMcounter = 0;  
  cycleFactor = &(RTCdata->cycle_factor);  

  // FIXME: Pass the variables to the Wifi Lib
  passWifiVariables( cycleFactor, RTCtimestamp );

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
