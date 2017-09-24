ESP8266 Daikin AC Remote

# Initialization
To initialize the project, use the following command:  
`git clone --recursive https://github.com/mat-cab/ESP8266-Daikin-Remote.git`

# Build the project
After having customized the project, use the following command:  
`make flash`

# External services
In order to analyse measurements online, the data is stored through Thingspeak.  
In order to update the schedule from the internet, a text file is retrieved from GIST.  

# Customization
The customization parameters are all set in the CustomConstants.h file present in the include/ folder.
This include the following parameters:
  * Wifi name and password
  * Thingspeak Channel ID and private key 
  * GIST URL of the schedule file

# Dependencies
This project is dependent on the makeEspArduino available here:  
https://github.com/plerup/makeEspArduino

It is also dependent on the IRremoteESP8266 library available here:  
https://github.com/markszabo/IRremoteESP8266
