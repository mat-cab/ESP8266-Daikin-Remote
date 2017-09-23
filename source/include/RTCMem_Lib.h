#ifndef RTCMem_Lib_H_
#define RTCMem_Lib_H_

#include "Arduino.h"

#include "Scheduler/Scheduler_RTCData.h"

// structures for the RTC memory
struct {
  uint32_t crc32; 
  byte data[508];
} rtcRawData;

// Usefull data in the RTC memory
// Note: this should be less than the 508 available bytes!
struct rtcData {
  // size of time_t: 
  time_t timestamp;
  // size of uint16_t: 2 bytes
  uint16_t iteration;
  // size of float: 
  float cycle_factor;
  // size of Scheduler_RTCData: 5 bytes
  Scheduler_RTCData schedulerRTCData;
};

bool readRTCmemory();

int32_t calculateCRC32(const uint8_t *data, size_t length);

int32_t getRTCmemCRC();

void writeRTCmem();

// Getters for the pointers
time_t * getRTCPointer_timestamp();
uint16_t * getRTCPointer_iteration();
float * getRTCPointer_cycleFactor();
Scheduler_RTCData * getRTCPointer_schedulerRTCData();
#endif
