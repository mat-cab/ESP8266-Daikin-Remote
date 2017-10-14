#ifndef SCHEDULER_ABSTRACT_ADDITIONAL_DATA_H_
#define SCHEDULER_ABSTRACT_ADDITIONAL_DATA_H_

#include "Arduino.h"

class AbstractAdditionalData {
  public:
  // Constructors

  // Getters
  virtual uint8_t * getRawData();
};

#endif
