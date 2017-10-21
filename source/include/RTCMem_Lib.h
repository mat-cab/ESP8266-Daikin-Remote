#ifndef RTCMem_Lib_H_
#define RTCMem_Lib_H_

#include "Arduino.h"

#include "Scheduler/Scheduler_RTCData.h"
#include "CycleManager/CycleManager_RTCData.h"

// structures for the RTC memory
struct {
  uint32_t crc32; 
  byte data[508];
} rtcRawData;

// Usefull data in the RTC memory
// Note: this should be less than the 508 available bytes!
struct rtcData {
  // size of Scheduler_RTCData: 5 bytes
  Scheduler_RTCData schedulerRTCData;
  // size of CycleManager_RTCData: 
  CycleManager_RTCData cycleManagerRTCData;
};

bool readRTCmemory();

int32_t calculateCRC32(const uint8_t *data, size_t length);

int32_t getRTCmemCRC();

void writeRTCmem();

// Getters for the pointers
Scheduler_RTCData * getRTCPointer_schedulerRTCData();
CycleManager_RTCData * getRTCPointer_cycleManagerRTCData();
#endif
