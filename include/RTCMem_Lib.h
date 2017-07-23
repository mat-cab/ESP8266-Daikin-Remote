#ifndef RTCMem_Lib_H_
#define RTCMem_Lib_H_

#include "Arduino.h"

// structures for the RTC memory
struct {
  uint32_t crc32; 
  byte data[508];
} rtcRawData;

// Usefull data in the RTC memory
// Note: this should be less than the 508 available bytes!
struct rtcData {
  time_t timestamp;
  uint16_t iteration;
  float cycle_factor;
};

bool readRTCmemory();

int32_t calculateCRC32(const uint8_t *data, size_t length);

int32_t getRTCmemCRC();

void writeRTCmem();

// Getters for the pointers
time_t * getRTCPointer_timestamp();
uint16_t * getRTCPointer_iteration();
float * getRTCPointer_cycleFactor();
#endif
