#include "IR_Remote_Daikin.h"

IR_Remote_Daikin::IR_Remote_Daikin() {
  this->IRremote = new IRDaikinESP(IR_PIN);
  this->IRremote->begin();
}

void IR_Remote_Daikin::setOn() {
  this->IRremote->on();
}

void IR_Remote_Daikin::setOff() {
  this->IRremote->off();
}

void IR_Remote_Daikin::setMode( AC_MODE acMode ) {
  switch (acMode) {
    case DRY:
      this->IRremote->setMode(kDaikinDry);
      break;
    case COOL:
      this->IRremote->setMode(kDaikinCool);
      break;
    case HEAT:
      this->IRremote->setMode(kDaikinHeat);
      break;
    case FAN:
      this->IRremote->setMode(kDaikinFan);
      break;
    default:
      this->IRremote->setMode(kDaikinAuto);
      break;
  }
}

void IR_Remote_Daikin::setTemperature( uint8_t temperature ) {
  this->IRremote->setTemp(temperature);
}

void IR_Remote_Daikin::setFanSpeed( AC_FANSPEED acFanSpeed ) {
  switch (acFanSpeed) {
    case LOW_FANSPEED:
      this->IRremote->setFan(kDaikinFanMin);
      break;
    case MEDIUM_FANSPEED:
      this->IRremote->setFan(kDaikinFanMed);
      break;
    case HIGH_FANSPEED:
      this->IRremote->setFan(kDaikinFanMax);
      break;
    default:
      this->IRremote->setFan(kDaikinFanAuto);
      break;
  }
}

void IR_Remote_Daikin::setBooleanOption( String optionName, bool optionValue ) {
  if (optionName.equals("SwingVertical")) {
    this->IRremote->setSwingVertical( optionValue );
  } else if (optionName.equals("SwingHorizontal")) {
    this->IRremote->setSwingHorizontal( optionValue );
  } else if (optionName.equals("Quiet")) {
    this->IRremote->setQuiet( optionValue );
  } else if (optionName.equals("Powerful")) {
    this->IRremote->setPowerful( optionValue );
  } else if (optionName.equals("Sensor")) {
    this->IRremote->setSensor( optionValue );
  } else if (optionName.equals("Econo")) {
    this->IRremote->setEcono( optionValue );
  } else if (optionName.equals("Mold")) {
    this->IRremote->setMold( optionValue );
  } else if (optionName.equals("Comfort")) {
    this->IRremote->setComfort( optionValue );
  }
}

void IR_Remote_Daikin::sendCommand() {
  this->IRremote->send();
}
