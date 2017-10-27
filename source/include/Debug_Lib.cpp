#include "Arduino.h"
#include "Time.h"
#include "Debug_Lib.h"

void initializeDebug() {
#ifdef DEBUG
  // Setup serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266 starting - Time is "+String(weekday())+" "+String(day())+"/"+String(month())+"/"+String(year())+" "+String(hour())+":"+String(minute())+":"+String(second()));
#endif
}

void endDebug() {
#ifdef DEBUG
  Serial.flush();

  Serial.end();
#endif
}

void debug(String message) {
#ifdef DEBUG
  Serial.println(message);
#endif
}

void debug(struct measurement *measure) {
#ifdef DEBUG
  char fConverter[10]; 
  
  dtostrf(getTemperature(measure), 0, 2, fConverter);
  debug( "Temperature : "+String(fConverter)+" degC");

  dtostrf(getHumidity(measure), 0, 2, fConverter);
  Serial.println("Humidity : "+String(fConverter)+" %HR");  

  dtostrf(getVoltage(measure), 0, 3, fConverter);
  Serial.println("Battery voltage : "+String(fConverter)+" V");

  Serial.println("Delta with last measure : "+String( getDelta(measure) )+" s");
#endif
}
