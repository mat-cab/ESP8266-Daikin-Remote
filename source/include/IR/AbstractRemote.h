#ifndef IR_ABSTRACT_REMOTE_H_
#define IR_ABSTRACT_REMOTE_H_

#include "Arduino.h"
#include "../AC/AC_Mode.h"
#include "../AC/AC_FanSpeed.h"

class AbstractRemote {
  public:
  // Constructors

  // Set the command options
  virtual void setOn();
  virtual void setOff();
  virtual void setMode( AC_MODE AcMode );
  virtual void setTemperature( uint8_t temperature );
  virtual void setFanSpeed( AC_FANSPEED acFanSpeed );
  virtual void setBooleanOption( String optionName, bool optionValue);

  // Send function
  virtual void sendCommand();  
};

#endif
