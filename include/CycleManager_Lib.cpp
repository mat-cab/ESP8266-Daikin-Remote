#include "Arduino.h"

#include "../libraries/Time/Time.h"

#include "RTCMem_Lib.h"
#include "CycleManager_Lib.h"

float *cycleFactor;
time_t *timestamp;
uint16_t *iteration;

void initializeCycleManager() {
  // Read the pointers from the RTC memory
  timestamp = getRTCPointer_timestamp();
  iteration = getRTCPointer_iteration();

  // Set the estimated local time
  setTime( *timestamp + (time_t)(CYCLE_TIME*(*iteration + 1)/1000));  
}

void resetCycleManager() {
  // reset the iteration to the last iteration
  // This way an update will be performed
  *iteration = CYCLE_ITERATIONS - 1;

  // Also reset the cycleFactor to the initial value
  *cycleFactor = CYCLE_FACTOR; 
}

void updateCycleManager() {
  // Compute the next iterations (in case of cycle overflow)
  *iteration = *iteration + (1 + millis() / CYCLE_TIME) % CYCLE_ITERATIONS;
}

uint32_t getNextCycle() {
  uint32_t waitMillis = CYCLE_TIME * (1 + millis() / CYCLE_TIME);
  uint32_t waitMicros = (waitMillis*1000-micros())*(*cycleFactor);

  return waitMicros;
}
