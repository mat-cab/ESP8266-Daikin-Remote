#include "IR_Remote_Coolix.h"

IR_Remote_Coolix::IR_Remote_Coolix() {
  this->IRremote = new IRCoolixAC(IR_PIN);
  this->IRremote->begin();
}

void IR_Remote_Coolix::setOn() {
  this->IRremote->on();
}

void IR_Remote_Coolix::setOff() {
  this->IRremote->off();
}

void IR_Remote_Coolix::setMode( AC_MODE acMode ) {
  switch (acMode) {
    case DRY:
      this->IRremote->setMode(kCoolixDry);
      break;
    case COOL:
      this->IRremote->setMode(kCoolixCool);
      break;
    case HEAT:
      this->IRremote->setMode(kCoolixHeat);
      break;
    case FAN:
      this->IRremote->setMode(kCoolixFan);
      break;
    default:
      this->IRremote->setMode(kCoolixAuto);
      break;
  }
}

void IR_Remote_Coolix::setTemperature( uint8_t temperature ) {
  this->IRremote->setTemp(temperature);
}

void IR_Remote_Coolix::setFanSpeed( AC_FANSPEED acFanSpeed ) {
  switch (acFanSpeed) {
    case LOW_FANSPEED:
      this->IRremote->setFan(kCoolixFanMin);
      break;
    case MEDIUM_FANSPEED:
      this->IRremote->setFan(kCoolixFanMed);
      break;
    case HIGH_FANSPEED:
      this->IRremote->setFan(kCoolixFanMax);
      break;
    default:
      this->IRremote->setFan(kCoolixFanAuto);
      break;
  }
}

void IR_Remote_Coolix::setBooleanOption( String optionName, bool optionValue ) {
  if (optionName.equals("Swing")) {
    if (optionValue)
      this->IRremote->setSwing();
  } else if (optionName.equals("Sleep")) {
    if (optionValue)
      this->IRremote->setSleep();
  } else if (optionName.equals("Turbo")) {
    if (optionValue)
      this->IRremote->setTurbo();
  } else if (optionName.equals("Led")) {
    if (optionValue)
      this->IRremote->setLed();
  } else if (optionName.equals("Clean")) {
    if (optionValue)
      this->IRremote->setClean();
  }
}

void IR_Remote_Coolix::sendCommand() {
  this->IRremote->send();
}
