#ifndef IR_LIB_H_
#define IR_LIB_H_

// Set output pin to 5 (GPIO 4)
#define IR_PIN 5

void initializeIR();

void sendIRCommand(uint8_t mode, uint8_t temperature, uint8_t fanSpeed, bool swingLR, bool swingUD, bool powerful, bool silent);

void sendIRStop();
#endif
