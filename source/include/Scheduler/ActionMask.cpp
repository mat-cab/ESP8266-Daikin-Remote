#include "AdditionalData_AC_START.h"
#include "AdditionalData_UPDATE_CYCLE.h"

#include "ActionMask.h"

ActionMask::ActionMask() {
  this->rawData = 0;
}

ActionMask::ActionMask(bool executed, ActionType actionType) {
  setActionType(actionType);
  setExecuted(executed);
}

ActionMask::ActionMask(uint8_t *dataPointer) {
  this->rawData = *dataPointer;
}

uint8_t * ActionMask::getRawData() {
  return &(this->rawData);
}

bool ActionMask::isExecuted() const {
  return ((this->rawData & 0x80) > 0);
}

uint8_t ActionMask::getSecondaryData() const {
  return ((this->rawData & 0x70) >> 4);
}

ActionType ActionMask::getActionType() const {
  return ((ActionType)(this->rawData & 0x0F));
}

bool ActionMask::isEmpty() const {
  return (getActionType() == EMPTY_ACTION);
}

void ActionMask::setActionType(ActionType actionType) {
  this->rawData = ((this->rawData & 0xF0) | (actionType & 0x0F));
  
  // If action type is AC_START, add two additional data words
  if (actionType == AC_START) {
    this->setSecondaryData(2);
  } else if ( actionType == UPDATE_CYCLE) {
  // If action type is UPDATE_CYCLE, add three additional data words
    this->setSecondaryData(3);
  }
}

void ActionMask::setExecuted(bool executed) {
  this->rawData = ((this->rawData & 0x7F ) | (executed & 0x01) << 7); 
}

void ActionMask::setSecondaryData(uint8_t info) {
  this->rawData = ((this->rawData & 0x8F) | (info & 0x07) << 4);
}
