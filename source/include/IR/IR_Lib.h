#ifndef IR_LIB_H_
#define IR_LIB_H_

#include "Arduino.h"

#include "../AC/AC_Mode.h"
#include "../AC/AC_FanSpeed.h"

#include "AbstractRemote.h"
#include "IR_Protocol.h"

// Set output pin to 5 (GPIO 4)
#define IR_PIN 5
// Set default protocol
#define IR_DEFAULT_PROTOCOL COOLIX_PROTOCOL

void initializeIR();

// Entry point to the remotes
void IRsetOn();
void IRsetOff();
void IRsetMode( AC_MODE acMode );
void IRsetTemperature( uint8_t temperature );
void IRsetFanSpeed( AC_FANSPEED acFanSpeed );
void IRsetBooleanOption( String optionName, bool optionValue );

void IRsendCommand();

IR_PROTOCOL getIRProtocol();
#endif
