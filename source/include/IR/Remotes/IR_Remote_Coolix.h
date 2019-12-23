#ifndef IR_REMOTE_COOLIX_H_
#define IR_REMOTE_COOLIX_H_

#include "Arduino.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Coolix.h"

#include "../../AC/AC_FanSpeed.h"
#include "../../AC/AC_Mode.h"

#include "../AbstractRemote.h"
#include "../IR_Lib.h"

class IR_Remote_Coolix : public AbstractRemote {
  private:
  IRCoolixAC *IRremote;

  public:
  IR_Remote_Coolix();
  void setOn();
  void setOff();
  void setMode( AC_MODE acMode );
  void setTemperature( uint8_t temperature );
  void setFanSpeed( AC_FANSPEED acFanSpeed );
  void setBooleanOption( String optionName, bool optionValue );

  void sendCommand();
};
#endif
