#ifndef IR_REMOTE_DAIKIN_H_
#define IR_REMOTE_DAIKIN_H_

#include "Arduino.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Daikin.h"

#include "../../AC/AC_Mode.h"
#include "../../AC/AC_FanSpeed.h"

#include "../AbstractRemote.h"
#include "../IR_Lib.h"

class IR_Remote_Daikin : public AbstractRemote {
  private:
  IRDaikinESP *IRremote;

  public:
  IR_Remote_Daikin();
  void setOn();
  void setOff();
  void setMode( AC_MODE acMode );
  void setTemperature( uint8_t temperature );
  void setFanSpeed( AC_FANSPEED acFanSpeed );
  void setBooleanOption( String optionName, bool optionValue );

  void sendCommand();
};
#endif
