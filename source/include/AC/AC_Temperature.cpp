#include "Arduino.h"

#include "ir_Daikin.h"
#include "ir_Coolix.h"

#include "../IR/IR_Lib.h"


uint8_t getProtocolTemperature(uint8_t temperature) {
  switch (getIRProtocol()) {
    case DAIKIN_PROTOCOL:
      return temperature - kDaikinMinTemp;
    case COOLIX_PROTOCOL:
      return temperature - kCoolixTempMin;
  }
}

uint8_t getAbsoluteTemperature(uint8_t protocolTemperature) {
  switch(getIRProtocol()) {
    case DAIKIN_PROTOCOL:
      return kDaikinMinTemp + protocolTemperature;
    case COOLIX_PROTOCOL:
      return kCoolixTempMin + protocolTemperature;
  }
}
