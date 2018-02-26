# find the root folder of the project
ROOT_FOLDER := $(shell echo $(CURDIR) | sed "s:\(.*ESP8266-Daikin-Remote/\).*:\1:")

# Specify the location of the sketch file
SKETCH := $(ROOT_FOLDER)/source/ESP8266-Daikin-Remote.ino

# Add the user libraries as additional lib
CUSTOM_LIBS := $(ROOT_FOLDER)/libraries/

include $(ROOT_FOLDER)/libraries/makeEspArduino/makeEspArduino.mk
