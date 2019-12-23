#include "../Debug_Lib.h"

#include "Action.h"

#include "Actions/EMPTY_ACTION/Action_EMPTY_ACTION.h"
#include "Actions/AC_START/Action_AC_START.h"
#include "Actions/AC_STOP/Action_AC_STOP.h"
#include "Actions/UPDATE_CYCLE/Action_UPDATE_CYCLE.h"
#include "Actions/UPDATE_SCHEDULE/Action_UPDATE_SCHEDULE.h"
#include "Actions/SEND_MEASURE/Action_SEND_MEASURE.h"

#include "ActionStaticFunctions.h"

Action * getNewAction(TimeMask * timeMask, ActionMask * aMask, uint8_t * aDataPointer) {
  switch (aMask->getActionType()) {
    case EMPTY_ACTION:
      return new Action_EMPTY_ACTION(timeMask, aMask, aDataPointer );
    case AC_START:
      return new Action_AC_START(timeMask, aMask, aDataPointer);
    case AC_STOP:
      return new Action_AC_STOP(timeMask, aMask, aDataPointer);
    case UPDATE_CYCLE:
      return new Action_UPDATE_CYCLE(timeMask, aMask, aDataPointer);
    case UPDATE_SCHEDULE:
      return new Action_UPDATE_SCHEDULE(timeMask, aMask, aDataPointer);
    case SEND_MEASURE:
      return new Action_SEND_MEASURE(timeMask, aMask, aDataPointer);
  }
}

Action * getNewAction(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) {
  switch (actionType) {
    case EMPTY_ACTION:
      return new Action_EMPTY_ACTION(actionType, dMask, hour, minute, second);
    case AC_START:
      return new Action_AC_START(actionType, dMask, hour, minute, second);
    case AC_STOP:
      return new Action_AC_STOP(actionType, dMask, hour, minute, second);
    case UPDATE_CYCLE:
      return new Action_UPDATE_CYCLE(actionType, dMask, hour, minute, second);
    case UPDATE_SCHEDULE:
      return new Action_UPDATE_SCHEDULE(actionType, dMask, hour, minute, second);
    case SEND_MEASURE:
      return new Action_SEND_MEASURE(actionType, dMask, hour, minute, second);
  }
}

Action * parseActionFromString(String actionString) {
  debug("Parsing action : "+actionString);

  // check if action is a comment
  if ( actionString[0] == '#' ) {  
    debug("Action is commented, it will not be taken into account");
    // exit
    return NULL;
  }

  char * argument = strtok(const_cast<char*>(actionString.c_str()),SCHEDULER_ACTION_PARSER_DELIMITER);
  ActionType aType;
  uint8_t argumentNumber = 0, daysMask = 0, hours = 0, minutes = 0, seconds = 0;
  Action * result = NULL;
  AbstractAdditionalData * aData = NULL;

  while (argument != NULL) {
    
    switch(argumentNumber) {
      case 0:
        // This is the action type
        aType = getActionType( argument );
        break;
      case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        // This is the days flag
        if ( strcmp( argument, "1" ) == 0) {
          // Inputs are Mon;Tues;Wed;Thu;Fri;Sat;Sun
          // Whereas daysMask is Sat;Sun;Mon;Tues;Wed;Thu;Fri;
          daysMask += (1 << ((argumentNumber + 1) % 7));
        }
        break;
      case 8:
        // This is for the hours
        hours = (uint8_t)atoi(argument);
        break;
      case 9:
        // This is for the minutes
        minutes = (uint8_t)atoi(argument);
        break;
      case 10:
        // This is for the seconds
        seconds = (uint8_t)atoi(argument);
        break;
      // The following arguments are additional arguments necessary for a few actions
    }

    if (argumentNumber == 10) {
      // The first mandatory options were given, create the associated action
      result = getNewAction(aType, daysMask, hours, minutes, seconds);
    }
    
    // if argument is more than 10, consider it as an additional argument for the action
    if (argumentNumber > 10) {
      result->addAdditionalActionData(argument,(uint8_t)(argumentNumber - 10));
    }

    // Loop for the next argument
    argumentNumber++;
    argument = strtok(NULL,SCHEDULER_ACTION_PARSER_DELIMITER);
  }
  
  // Say there was an error for unknown actions
  if (aType == UNKNOWN_ACTION) {
    debug("Error while parsing this action!");

    result = NULL;
  }

  return result;
}
