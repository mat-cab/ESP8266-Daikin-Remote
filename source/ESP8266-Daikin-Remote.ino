// System includes
#include "Arduino.h"

// External lib includes
#include "Time.h"

// Project includes
#include "include/CustomConstants.h"
#include "include/I2C_Lib.h"
#include "include/Measurement.h"
#include "include/SHT31_Lib.h"
#include "include/RTCMem_Lib.h"
#include "include/ExPROM/ExPROM.h"
#include "include/Battery_Lib.h"
#include "include/Wifi/Wifi.h"
#include "include/CycleManager/CycleManager.h"
#include "include/Debug_Lib.h"
#include "include/Scheduler/Scheduler.h"
#include "include/IR_Lib.h"

void setup() {
  // Save the power
  disconnectWifi();

  // Initial declaration
  uint16_t *iteration;

  // Read the RTC memory
  bool corruptedRTCmem = readRTCmemory();

  /* -----------------------------------------------
      INITIALIZATION OF ALL MODULES
      Must start by the cycleManager
  -------------------------------------------------- */
  
  // initialize the cycleManager
  initializeCycleManager();

  // Init the debug
  initializeDebug();

  // Init the pins for the battery measurement
  initializeBatteryMeasurement();

  // Init the I2C protocol
  initializeI2C();

  // Init the Scheduler
  initializeScheduler();

  // Init the IR 
  initializeIR();

  // Now that time is set, update other less usefull values
  measurement measure;  
  iteration = getRTCPointer_cycleManagerRTCData()->getIteration();

  // Also reset initialize the wifi library
  initializeWifi();

  /* -----------------------------------------------
      RESET OF THE MODULES
      In case of corrupted RTC memory
  -------------------------------------------------- */

  // If memory is corrupted
  if (corruptedRTCmem) {
     // Also send to debug
    debug("RTC memory is corrupted");

    // reset the cycle manager
    resetCycleManager();

    // reset the EEPROM
    resetEEPROM();

    // reset the scheduler
    resetScheduler();

    // reset the wifi
    resetWifi();
  }

  // send the schedule to the debug
  printSchedule();  

  // Pause before looping
  delay(100);

  
  /* -----------------------------------------------
      CYCLE EXECUTION
      Measurement is performed and saved to EEPROM
      Scheduler is run
  -------------------------------------------------- */

  // TODO: Add the possibility to enable/disable measurements

  // Send debug message
  debug("Performing measurements ...");

  // Perform the measurements
  performMeasurement(&measure);

  // Send to debug
  debug(&measure);
  debug("Saving in EEPROM...");

  // Save the measurement in EEPROM
  writeMeasurementInEEPROM(&measure);

  // Run the scheduler and all scheduled action
  runScheduler();

  // Disconnect from the Wifi
  disconnectWifi();

  // write what is necessary in the EEPROM
  writeEEPROM();

  /* -----------------------------------------------
      CYCLE END
      Close the debug
      Save all necessary info
      Go to deepSleep
  -------------------------------------------------- */

  // Send message to debug
  debug("*** End of iteration "+ String(*iteration) +" - Time is "+String(weekday())+" "+String(day())+"/"+String(month())+"/"+String(year())+" "+String(hour())+":"+String(minute())+":"+String(second())+" ***");

  // Close the debug
  endDebug();

  // Update the cycle manager
  updateCycleManager();

  // Write the RTC memory data
  writeRTCmem();  
  
  // Go do deepsleep until the cycle manager says so
  // TODO: Improve the deepSleep management to allow for longer deep sleeps (>71 minutes)
  ESP.deepSleep(getNextCycle());
}

void loop() {
  // Empty loop for the deepSleep
}
