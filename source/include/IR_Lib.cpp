#include "Arduino.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Daikin.h"

#include "Debug_Lib.h"

#include "IR_Lib.h"

// Create structure for daikin remote
IRDaikinESP daikinir(IR_PIN);

void initializeIR() {
  debug("Initializing IR library...");
  
  daikinir.begin();

  debug("Done!");
}

void testIR() {
  debug("Setting up IR signal...");

  // Set AC to ON
  daikinir.on();

  // Set fan to auto
  daikinir.setFan(0);

  // Set temperature to 26
  daikinir.setTemp(26);

  // set mode to cool
  daikinir.setMode(DAIKIN_COOL);

  // set swings to no
  daikinir.setSwingVertical(0);
  daikinir.setSwingHorizontal(0);

  debug("Sending IR signal...");

  // Send the signal
  daikinir.send();
}
