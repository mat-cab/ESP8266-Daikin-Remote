#ifndef RTCMem_Lib_H_
#define RTCMem_Lib_H_

#include "Arduino.h"

// structures for the RTC memory
struct rtcRawDataDef {
  uint32_t crc32; 
  byte data[508];
};

extern rtcRawDataDef rtcRawData;

struct rtcData {
  time_t timestamp;
  uint16_t iteration;
  float cycle_factor;
};

int32_t calculateCRC32(const uint8_t *data, size_t length);

int32_t getRTCmemCRC();

void writeRTCmem();
#endif
