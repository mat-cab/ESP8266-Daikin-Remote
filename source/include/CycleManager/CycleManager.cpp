#include "Arduino.h"

#include "Time.h"

#include "../Scheduler/Scheduler.h"
#include "../Debug_Lib.h"
#include "../CustomConstants.h"
#include "../RTCMem_Lib.h"
#include "CycleManager_RTCData.h"
#include "CycleManager.h"

CycleManager_RTCData * cycleManagerRTCData;
// Data from the RTC memory
float *cycleFactor;
time_t *timestamp;
time_t *timeElapsed;
uint64_t *cycleTime;
uint64_t *remainingSleepTime;
uint32_t *timeSlept;

// Internal data
uint32_t currentSleepTime;
bool reset;

void initializeCycleManager(bool corruptedMemory) {
  // Read the pointer from the RTC memory
  cycleManagerRTCData = getRTCPointer_cycleManagerRTCData();

  // Read the pointers from the RTC memory
  timestamp = cycleManagerRTCData->getTimestamp();
  timeElapsed = cycleManagerRTCData->getTimeElapsed();

  // Check whether the memory is corrupted or not
  if ( !corruptedMemory ) {
    // Set the estimated local time
    setTime( *timestamp + *timeElapsed ); 
  }

  // Read the remaining sleep time from the RTC memory 
  remainingSleepTime = cycleManagerRTCData->getRemainingSleepTime();

  // Verify that the memory is not corrupted and whether we still need to deepsleep
  if ( !corruptedMemory && *remainingSleepTime > 0 ) {
    // update the deep sleep timers
    // Set the remainingSleepTime properly (do not overflow CYCLE_MAX_DEEPSLEEP_TIME)
    updateDeepSleepTimers(*remainingSleepTime);

    // save the RTC memory
    writeRTCmem(); 

    // continue the deepsleep
    goToDeepSleep();
  }

  // initialize with no reset
  reset = false;

  // Read cycleTime pointer from RTC memory
  cycleTime = cycleManagerRTCData->getCycleTime();
  // Read cycleFactor pointer from RTC memory
  cycleFactor = cycleManagerRTCData->getCycleFactor();
  // Read the time that we slept for
  timeSlept = cycleManagerRTCData->getTimeSlept();
}

void resetCycleManager() {
  // reset the timestamp to 0
  *timestamp = 0;

  // reset the time elapsed to 0
  *timeElapsed = 0;

  // Also reset the cycleFactor to the initial value
  *cycleFactor = CYCLE_FACTOR; 

  // Reset the cycleTime to the initial value
  *cycleTime = CYCLE_TIME;

  // reset the remaining sleep time to 0
  *remainingSleepTime = 0;

  // reset the time slept to 0
  *timeSlept = 0;

  // set the reset flag to true
  reset = true;
}

void updateCycleManager() {
  // Find how long we need to wait for the next action (in s)
  // Add a second to make sure we will wake up after the scheduled action
  uint64_t nextActionWaitTime = remainingTimeBeforeNextAction() + 1;
  
  // Find how long we need to wait for the next cycle (in s)
  // note that we take into account any cycle overflow
  uint64_t nextCycleWaitTime = *cycleTime * ( 1 + millis() / (*cycleTime * 1000));

  // get the minimal one
  // note that if scheduler is not strict, then it will reply that next action is due in forever
  if (nextActionWaitTime > nextCycleWaitTime) {
    // Save that we will sleep for nextCycleWaitTime
    *timeSlept = nextCycleWaitTime;

    // Also increase the timeElapsed
    *timeElapsed += nextCycleWaitTime;

    // update the deep sleep timers to the next cycle
    // Convert to us and use the cycleFactor
    // Also adjust with what has already passed in this cycle
    updateDeepSleepTimers( (nextCycleWaitTime * 1E6 - micros()) * (*cycleFactor) );
  } else {
    // Save that we will sleep for nextActionWaitTime
    *timeSlept = nextActionWaitTime;

    // Also increase the timeElapsed
    *timeElapsed += nextActionWaitTime;

    // update the deep sleep timers to the next action
    // Convert to us and use the cycleFactor
    updateDeepSleepTimers( nextActionWaitTime * 1E6 * (*cycleFactor) );
  }
}

void updateDeepSleepTimers(uint64_t sleepTime) {
    // Make sure that currentSleepTime does not overflow CYCLE_MAX_DEEPSLEEP_TIME
    if ( sleepTime > CYCLE_MAX_DEEPSLEEP_TIME ) {
      // We need to sleep for a long time
      currentSleepTime = CYCLE_MAX_DEEPSLEEP_TIME;

      // set the remaining sleep time accordingly
      *remainingSleepTime = sleepTime - CYCLE_MAX_DEEPSLEEP_TIME;
    } else {
      // we are just about to wake up (last deepsleep)
      currentSleepTime = sleepTime;

      // set that we no longer need to deepsleep
      *remainingSleepTime = 0;
    }
}

void goToDeepSleep() {
    // Go to deepsleep for the currentSleepTime
    ESP.deepSleep(currentSleepTime, WAKE_NO_RFCAL);
}

void updateCycleFactor(int32_t timeShift, uint32_t timeSpan) {
  // Do not update if there was a reset of the cycleManager  
  if (!reset) {  
    // Update wrt last update
    *cycleFactor -= (float)timeShift / (float)(timeSpan);
  }
}

void updateCycleTime(uint32_t newCycleTime) {
  debug("Updating cycle time");

  debug("Setting new cycle time to "+String(newCycleTime));
  
  // set the new cycle time (in s)
  *cycleTime = newCycleTime;
} 

int32_t updateTime(String timestampString) {
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

  // Also send to debug
  debug("Adjusted internal time!");

  // Compute for the time shift between the two dates
  int32_t shift =  newTime - lastTimestamp;

  // if there is a bit of time between the two updates
  if ((lastTimestamp - *timestamp) > CYCLE_ADJUST_FACTOR_MIN_TIME) {
    // Adjust cycle factor wrt last time update
    updateCycleFactor(shift, (uint32_t) (lastTimestamp - *timestamp));

    // Ouput the time shift  
    debug("Shift was: "+String(shift)+" seconds on a span of "+String((lastTimestamp - *timestamp))+" s"); 

    // Also output the adjusted cycleFactor
    debug("Adjusted cycle factor to: "+String(*cycleFactor));
  }

  // Set the time in the RTC memory
  // Note: the cycle time is the time at the beginning of the iteration
  // Therefore it is necessary to remove the millis since the start of the iteration
  *timestamp = newTime - (currentMillis/(uint32_t)1000);

  // Also reset to 0 the timeElapsed
  *timeElapsed = 0;

  // return the shift for other timers adjustement
  return shift;
}

uint16_t getCycleTime() {
  return *cycleTime;
}

uint32_t getTimeSlept() {
  return *timeSlept;
}

time_t getCurrentCycleStart() {
  uint32_t currentMillis = (uint32_t)(millis())/((uint32_t)1000);
  time_t timeNow = now();
  return timeNow - currentMillis;
}
