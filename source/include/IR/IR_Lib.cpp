#include "Arduino.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Daikin.h"
#include "ir_Coolix.h"

#include "Remotes/IR_Remote_Daikin.h"
#include "Remotes/IR_Remote_Coolix.h"
#include "../Debug_Lib.h"

#include "IR_Lib.h"

AbstractRemote *remote;

void initializeIR() {
  debug("Initializing IR library...");

  switch (IR_DEFAULT_PROTOCOL) {
    case COOLIX_PROTOCOL:
      remote = new IR_Remote_Coolix();
    default:
      remote = new IR_Remote_Daikin();
  }

  debug("Done!");
}

void IRsetOn() {
  remote->setOn();
}

void IRsetOff() {
  remote->setOff();
} 

void IRsetMode( AC_MODE acMode ) {
  remote->setMode( acMode );
}

void IRsetTemperature( uint8_t temperature ) {
  remote->setTemperature( temperature );
}

void IRsetFanSpeed( AC_FANSPEED acFanSpeed ) {
  remote->setFanSpeed( acFanSpeed );
}

void IRsetBooleanOption( String optionName, bool optionValue ) {
  remote->setBooleanOption( optionName, optionValue );
}

void IRsendCommand() {
  debug("Sending IR command ...");

  remote->sendCommand();

  debug("IR command sent!");
}

IR_PROTOCOL getIRProtocol() {
  return IR_DEFAULT_PROTOCOL;
}
