# find the root folder of the project
ROOT_FOLDER := $(shell echo $(CURDIR) | sed "s:\(.*ESP8266-Daikin-Remote/\).*:\1:" | sed "s:/*$$::")

# Specify the location of the sketch file
SKETCH := $(ROOT_FOLDER)/source/ESP8266-Daikin-Remote.ino

# Add the user libraries as additional lib
LIBS := $(ROOT_FOLDER)/libraries/

# Exclude the test files for IRremote and Time libraries
EXCLUDE_DIRS := $(ROOT_FOLDER)/libraries/(IRremoteESP8266|Time)/(test|examples)/*

# Special flags for the IRremote library
BUILD_SPECIAL_FLAGS := "-D_IR_ENABLE_DEFAULT_=false -DSEND_DAIKIN=true -DSEND_COOLIX=true"

include $(ROOT_FOLDER)/libraries/makeEspArduino/makeEspArduino.mk
