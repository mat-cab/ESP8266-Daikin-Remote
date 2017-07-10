/*
  CustomConstants.h
  User specific constants (Wifi name and password, Thingspeak channel...)
*/

#ifndef CustomConstants_H_
#define CustomConstants_H_

// Private constants for the Timezone
// Format is <sign><2 digits hours><2 digits minutes>
// Example for UTC - 3, "-0300"
// Example for UTC + 1, "+0100"
#define TIMEZONE String("+0100")

// Private constants for the Wifi
// Wifi SSID to connect to
#define WIFI_SSID "WIFI_SSI"
// Wifi Password
#define WIFI_PASSWD "WIFI_PASSWORD"

// Private constants for Thingspeak channel
// ID of the channel
#define IOT_CHANNEL_ID "00000"
// Key of the stream
#define IOT_STREAM_KEY String("0000000000000000")
#endif
