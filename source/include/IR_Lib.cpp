#include "Arduino.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Daikin.h"
#include "ir_Coolix.h"

#include "Debug_Lib.h"

#include "IR_Lib.h"

// Create structure for daikin remote
//IRDaikinESP daikinir(IR_PIN);
IRCoolixAC daikinir(IR_PIN);

void initializeIR() {
  debug("Initializing IR library...");
  
  daikinir.begin();

  debug("Done!");
}

void sendIRCommand(uint8_t mode, uint8_t temperature, uint8_t fanSpeed, bool swingLR, bool swingUD, bool powerful, bool silent) {
  debug("Setting up IR signal...");

  // Set AC to ON
  daikinir.on();

  // Set fan
  daikinir.setFan(fanSpeed);

  // Set temperature
  daikinir.setTemp(temperature);

  // set mode
  daikinir.setMode(mode);

  // set swings
/*  daikinir.setSwingVertical(swingUD);
  daikinir.setSwingHorizontal(swingLR);

  // set auxiliaries
  daikinir.setPowerful(powerful);
  daikinir.setQuiet(silent);*/

  debug("Sending IR signal...");

  // Send the signal
  daikinir.send();
}

void sendIRStop() {
  debug("Setting up IR signal...");

  // Set AC to OFF
  daikinir.off();

  debug("Sending IR signal...");

  // Send the signal
  daikinir.send();
}
