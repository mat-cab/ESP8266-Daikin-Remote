#include "Arduino.h"
#include "RTCMem_Lib.h"

int32_t calculateCRC32(const uint8_t *data, size_t length)
{
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

void writeRTCmem()
{
  // Update CRC32 of data
  rtcRawData.crc32 = getRTCmemCRC();
  // Write struct to RTC memory
  ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcRawData, sizeof(rtcRawData));
}

