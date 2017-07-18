ESP8266 Daikin AC Remote

# Initialization
To initialize the project, use the following command:  
`git clone --recursive https://github.com/mat-cab/ESP8266-Daikin-Remote.git`

# Build the project
After having customized the project, use the following command:  
`make flash`

# Customization
The customization parameters are all set in the CustomConstants.h file present in the include/ folder.
This include the following parameters:
  * Wifi name and password
  * Thingspeak Channel ID and private key 

# Dependencies
This project is dependent on the makeEspArduino available here:  
https://github.com/plerup/makeEspArduino
