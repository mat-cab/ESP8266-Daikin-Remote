#include "Arduino.h"
#include "Time.h"
#include "Debug_Lib.h"

boolean debug_mode = false;

void initializeDebug() {
  // Set the Debug pin as input
  pinMode(DEBUG_PIN, INPUT);

  // check if debug mode is activated on Debug pin
  debug_mode = (digitalRead(DEBUG_PIN) == LOW);

  if (debug_mode) {
    // Setup serial
    Serial.begin(DEBUG_SPEED);
    Serial.println();
    Serial.println("ESP8266 starting - Time is "+String(weekday())+" "+String(day())+"/"+String(month())+"/"+String(year())+" "+String(hour())+":"+String(minute())+":"+String(second()));
  }
}

void endDebug() {
  if (debug_mode) {
    Serial.flush();

    Serial.end();
  }
}

void debug(String message) {
  if (debug_mode) {
    Serial.println(message);
  }
}

void debug(struct measurement *measure) {
  if (debug_mode) {
    char fConverter[10]; 
  
    dtostrf(getTemperature(measure), 0, 2, fConverter);
    debug( "Temperature : "+String(fConverter)+" degC");

    dtostrf(getHumidity(measure), 0, 2, fConverter);
    Serial.println("Humidity : "+String(fConverter)+" %HR");  

    dtostrf(getVoltage(measure), 0, 3, fConverter);
    Serial.println("Battery voltage : "+String(fConverter)+" V");

    Serial.println("Delta with last measure : "+String( getDelta(measure) )+" s");
  }
}
