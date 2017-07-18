#include "Measurement.h"
#include "Battery_Lib.h"

void initializeBatteryMeasurement() {
  // Set the voltage measurement pins appropriately
  pinMode(BATCHECK_READ, INPUT);
  pinMode(BATCHECK_ACT, OUTPUT);
}

void measureBattery(struct measurement *measureDatastore) {
  //
  uint16_t ADCsum;
  ADCsum = 0;

  int ADCraw;
  
  // Activate Battery reader
  digitalWrite(BATCHECK_ACT, HIGH);
  
  // wait a litlle
  delay(BATCHECK_DELAY);

  // Loop to have a mean value
  for ( int ADCmeasures = 0; ADCmeasures < BATCHECK_LOOPS; ADCmeasures++ ) {
    // Read the ADC
    ADCsum += analogRead(BATCHECK_READ);

    // wait before next iteration
    delay(BATCHECK_LOOP_DELAY);
  }
  
  // Disable battery reader
  digitalWrite(BATCHECK_ACT, LOW);

  // Have a mean value
  measureDatastore->voltage = ADCsum / BATCHECK_LOOPS;
}

