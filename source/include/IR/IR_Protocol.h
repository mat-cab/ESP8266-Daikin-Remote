#ifndef IR_PROTOCOL_H_
#define IR_PROTOCOL_H_

#include "Arduino.h"

// Enum for the IR protocols
enum IR_PROTOCOL : uint8_t {
  DAIKIN_PROTOCOL = 0,
  COOLIX_PROTOCOL
};

String printIRProtocol( IR_PROTOCOL Protocol );

IR_PROTOCOL getIRProtocolFromString( String IrProtocol ); 
#endif
