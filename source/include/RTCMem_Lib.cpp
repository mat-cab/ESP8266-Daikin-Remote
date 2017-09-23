#include "Arduino.h"
#include "RTCMem_Lib.h"

struct rtcData *RTCdata;

bool readRTCmemory() {
  boolean corruptedMem;

  // Read the whole RTC memory
  ESP.rtcUserMemoryRead(0, (uint32_t*) &rtcRawData, sizeof(rtcRawData));

  // Find whether RTC memory is corrupted or not
  corruptedMem = (rtcRawData.crc32 != getRTCmemCRC());

  // Initialize pointers for correct structure
  RTCdata = (struct rtcData*)&(rtcRawData.data);

  // return whether RTC memory is corrupted
  return corruptedMem;
}

int32_t calculateCRC32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xffffffff;
  while (length--) {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if (c & i) {
        bit = !bit;
      }
      crc <<= 1;
      if (bit) {
        crc ^= 0x04c11db7;
      }
    }
  }
  return crc;
}

int32_t getRTCmemCRC() {
  return calculateCRC32(((uint8_t*) &rtcRawData) + 4, sizeof(rtcRawData) - 4);
}

void writeRTCmem() {
  // Update CRC32 of data
  rtcRawData.crc32 = getRTCmemCRC();
  // Write struct to RTC memory
  ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcRawData, sizeof(rtcRawData));
}

time_t * getRTCPointer_timestamp() {
  return &(RTCdata->timestamp);
}

uint16_t * getRTCPointer_iteration() {
  return &(RTCdata->iteration);
}

float * getRTCPointer_cycleFactor() {
  return &(RTCdata->cycle_factor);
}

TimeMask * getRTCPointer_nextActionTimeMask() {
  return (&RTCdata->nextActionTimeMask);
}

uint8_t * getRTCPointer_lastSchedulerDay() {
  return (&RTCdata->lastSchedulerDay);
}
