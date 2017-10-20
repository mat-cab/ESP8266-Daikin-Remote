#include "Arduino.h"

#include "Time.h"

#include "RTCMem_Lib.h"
#include "Debug_Lib.h"
#include "CustomConstants.h"
#include "CycleManager_Lib.h"

float *cycleFactor;
time_t *timestamp;
uint16_t *iteration;
bool reset;

void initializeCycleManager() {
  // Read the pointers from the RTC memory
  timestamp = getRTCPointer_timestamp();
  iteration = getRTCPointer_iteration();

  // Set the estimated local time
  setTime( *timestamp + (time_t)(CYCLE_TIME*(*iteration + 1)));  

  // initialize with no reset
  reset = false;

  // Read cycleFactor pointer from RTC memory
  cycleFactor = getRTCPointer_cycleFactor();
}

void resetCycleManager() {
  // reset the iteration to the last iteration
  // This way an update will be performed
  *iteration = CYCLE_ITERATIONS - 1;

  // Also reset the cycleFactor to the initial value
  *cycleFactor = CYCLE_FACTOR; 

  // set the reset flag to true
  reset = true;
}

void updateCycleManager() {
  // Compute the next iterations (in case of cycle overflow)
  *iteration = (*iteration + (1 + millis() / (CYCLE_TIME * 1000))) % CYCLE_ITERATIONS;
}

uint32_t getNextCycle() {
  uint32_t waitMillis = CYCLE_TIME * 1000 * (1 + millis() / (CYCLE_TIME * 1000));
  uint32_t waitMicros = (waitMillis*1000-micros())*(*cycleFactor);

  return waitMicros;
}

void updateCycleFactor(uint32_t timeShift) {
  // Do not update if there was a reset of the cycleManager  
  if (!reset) {
    // Update wrt last update
    *cycleFactor -= (float)timeShift * 1000.0 / (float)(CYCLE_TIME * 1000 * CYCLE_ITERATIONS);
  }
}

void updateTime(String timestampString) {
  time_t lastTimestamp = now();
  uint32_t currentMillis = millis();
  
  TimeElements newTimeTE;
  time_t newTime;

  // month conversion from String to int
  int month;
  String monthString = timestampString.substring(8,11);

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
  newTimeTE.Year = timestampString.substring(12,16).toInt()-1970;
  newTimeTE.Month = month;
  newTimeTE.Day = timestampString.substring(5,7).toInt();
  newTimeTE.Hour = timestampString.substring(17,19).toInt();
  newTimeTE.Minute = timestampString.substring(20,22).toInt();
  newTimeTE.Second = timestampString.substring(23,25).toInt();
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
  updateCycleFactor(shift);

  // Ouput the time shift  
  debug("Shift was: "+String(shift)+" seconds"); 

  // Also output the adjusted cycleFactor
  debug("Adjusted cycle factor to: "+String(*cycleFactor));

  // Set the time in the RTC memory
  // Note: the cycle time is the time at the beginning of the iteration
  // Therefore it is necessary to remove the millis since the start of the iteration
  *timestamp = newTime - ((currentMillis-1000)/1000);
}

time_t getMeasurementTime(struct measurement * measure) {
  return *timestamp + measure->iterationMoment * CYCLE_TIME;
}
