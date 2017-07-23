// System includes
#include "Arduino.h"

// External lib includes
#include "libraries/Time/Time.h"

// Project includes
#include "include/CustomConstants.h"
#include "include/I2C_Lib.h"
#include "include/Measurement.h"
#include "include/SHT31_Lib.h"
#include "include/RTCMem_Lib.h"
#include "include/EEPROM_Measurement_Lib.h"
#include "include/Battery_Lib.h"
#include "include/Wifi_Lib.h"
#include "include/CycleManager_Lib.h"
#include "include/Debug_Lib.h"

void setup() {
  // Save the power
  initializeWifi();

  // Initial declaration
  uint16_t *iteration;

  // Read the RTC memory
  bool corruptedRTCmem = readRTCmemory();

  // initialize the cycleManager
  initializeCycleManager();

  // Init the debug
  initializeDebug();

  // If memory is corrupted
  if (corruptedRTCmem) {
    // reset the cycle manager (force an autoupdate)
    resetCycleManager();
  }

  // Init the pins for the battery measurement
  initializeBatteryMeasurement();

  // Init the I2C protocol
  initializeI2C();

  // Now that time is set, update other less usefull values
  measurement measure;  
  iteration = getRTCPointer_iteration();

  // In case of reset, fix the initial values
  if (corruptedRTCmem) {  
    // Also send to debug
    debug("RTC memory is corrupted");

    // reset the EEPROM
    resetEEPROM();
  }  
    
  // Pause before looping
  delay(100);

  // Send debug message
  debug("Performing measurements ...");

  // Perform the measurements
  performMeasurement(&measure, *iteration);

  // Send to debug
  debug(&measure);
  debug("Saving in EEPROM...");

  // Save the measurement in EEPROM
  writeMeasurementInEEPROM(&measure);

  // If at end of cycle
  if ( *iteration == CYCLE_ITERATIONS - 1 ) {
    // Send the data with Wifi
    sendWifi();

    // reset the EEPROM
    resetEEPROM(); 
  }

  // Send message to debug
  debug("*** End of iteration "+ String(*iteration) +" - Time is "+String(weekday())+" "+String(day())+"/"+String(month())+"/"+String(year())+" "+String(hour())+":"+String(minute())+":"+String(second())+" ***");

  // Close the debug
  endDebug();

  // Update the cycle manager
  updateCycleManager();

  // Write the RTC memory data
  writeRTCmem();  
  
  // Go do deepsleep until the cycle manager says so
  ESP.deepSleep(getNextCycle());
}

void loop() {
  // Empty loop for the deepSleep
}
