#include "IR_Protocol.h"

String printIRProtocol( IR_PROTOCOL Protocol ) {
  String result;

  switch (Protocol) {
    case COOLIX_PROTOCOL:
      result = "COOLIX";
      break;
    default:
      result = "DAIKIN";
      break;
  }

  return result;
}

IR_PROTOCOL getIRProtocolFromString( String IrProtocol ) {
  IR_PROTOCOL result;

  if ( IrProtocol.equals("COOLIX") ) {
    result = COOLIX_PROTOCOL;
  } else {
    result = DAIKIN_PROTOCOL;
  }

  return result;
}
