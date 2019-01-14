#include "Arduino.h"

#include "Time.h"

#include "../Debug_Lib.h"
#include "../CustomConstants.h"
#include "../RTCMem_Lib.h"
#include "CycleManager_RTCData.h"
#include "CycleManager.h"

CycleManager_RTCData * cycleManagerRTCData;
float *cycleFactor;
time_t *timestamp;
time_t *lastUpdateTimestamp;
uint16_t *iteration;
uint16_t *lastIteration;
uint64_t *cycleTime;
uint64_t *remainingSleepTime;
uint32_t currentSleepTime;
bool reset;

void initializeCycleManager() {
  // Read the pointer from the RTC memory
  cycleManagerRTCData = getRTCPointer_cycleManagerRTCData();

  // Read the pointers from the RTC memory
  timestamp = cycleManagerRTCData->getTimestamp();
  iteration = cycleManagerRTCData->getIteration();
  cycleTime = cycleManagerRTCData->getCycleTime();

  // Set the estimated local time
  setTime( *timestamp + (time_t)(*cycleTime*(*iteration))); 

  // Read the remaining sleep time from the RTC memory 
  remainingSleepTime = cycleManagerRTCData->getRemainingSleepTime();

  // Verify there is still some time to sleep
  if ( *remainingSleepTime > 0 ) {
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

  // Read cycleFactor pointer from RTC memory
  cycleFactor = cycleManagerRTCData->getCycleFactor();
  // Read the last iteration from RTC memory
  lastIteration = cycleManagerRTCData->getLastIteration();
  // Read the last updateTime timestamp
  lastUpdateTimestamp = cycleManagerRTCData->getLastUpdateTimestamp();
}

void resetCycleManager() {
  // Also reset the cycleFactor to the initial value
  *cycleFactor = CYCLE_FACTOR; 

  // Reset the cycleTime to the initial value
  *cycleTime = CYCLE_TIME;

  // reset the iterations to 0
  *iteration = 0;

  // reset the last iteration to 0
  *lastIteration = 0;

  // reset the remaining sleep time to 0
  *remainingSleepTime = 0;

  // set the reset flag to true
  reset = true;
}

void updateCycleManager() {
  // Save the last iteration
  *lastIteration = *iteration;

  // Compute the next iterations (in case of cycle overflow)
  *iteration = *iteration + (1 + (uint32_t)millis() / (uint32_t)(*cycleTime * 1000));
  
  // Find how long we need to wait for the next cycle
  // TODO: support the wake up at the next action (and not necessarly on a cycle)
  uint64_t nextCycleWaitTime = getNextCycle();
  
  // update the deep sleep timers
  updateDeepSleepTimers( nextCycleWaitTime );
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

uint64_t getNextCycle() {
  uint64_t waitMillis = *cycleTime * 1000 * getIterationsFromLastCycle();
  uint64_t waitMicros = (waitMillis*1000-micros())*(*cycleFactor);

  return waitMicros;
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

  // save the cycle start
  time_t cycleStart = getCurrentCycleStart();

  // save the number of cycles since the last cycle
  uint16_t lastCycleIteration = getIterationsFromLastCycle();
 
  debug("Setting new cycle time to "+String(newCycleTime));
  // set the new cycle time
  *cycleTime = newCycleTime;

  // reset the last iteration to 0
  *lastIteration = 0;
  // reset the iterations to 1 (to avoid having 0 iterations in a cycle)
  *iteration = lastCycleIteration;

  // reset the timestamp to the cycle start
  *timestamp = cycleStart;
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
  if ((lastTimestamp - *lastUpdateTimestamp) > CYCLE_ADJUST_FACTOR_MIN_TIME) {
    // Adjust cycle factor wrt last time update
    updateCycleFactor(shift, (uint32_t) (lastTimestamp - *lastUpdateTimestamp));

    // Ouput the time shift  
    debug("Shift was: "+String(shift)+" seconds on a span of "+String((lastTimestamp - *lastUpdateTimestamp))+" s"); 

    // Also output the adjusted cycleFactor
    debug("Adjusted cycle factor to: "+String(*cycleFactor));
  }

  // Set the time in the RTC memory
  // Note: the cycle time is the time at the beginning of the iteration
  // Therefore it is necessary to remove the millis since the start of the iteration
  *timestamp = newTime - (currentMillis/(uint32_t)1000);

  // also set the lastUpdateTimestamp
  *lastUpdateTimestamp = *timestamp;
  
  // save the number of cycles since the last cycle
  uint16_t lastCycleIteration = getIterationsFromLastCycle();

  // reset the last iteration to 0
  *lastIteration = 0;
  // reset the iterations to 0
  *iteration = 0;

  // return the shift for other timers adjustement
  return shift;
}

uint16_t getCycleTime() {
  return *cycleTime;
}

uint16_t getIterationsFromLastCycle() {
  return (*iteration - *lastIteration);
}

time_t getCurrentCycleStart() {
  uint32_t currentMillis = (uint32_t)(millis())/((uint32_t)1000);
  time_t timeNow = now();
  return timeNow - currentMillis;
}
