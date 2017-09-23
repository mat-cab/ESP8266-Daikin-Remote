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

void ActionMask::setActionType(ActionType actionType) {
  this->rawData = ((this->rawData & 0xF0) | (actionType & 0x0F));
}

void ActionMask::setExecuted(bool executed) {
  this->rawData = ((this->rawData & 0x7F ) | (executed & 0x01) << 7); 
}

void ActionMask::setSecondaryData(uint8_t info) {
  this->rawData = ((this->rawData & 0x8F) | (info & 0x07) << 4);
}
