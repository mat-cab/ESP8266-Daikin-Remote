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
// State if Static IP must be used on this Wifi (or let DHCP decide everything)
// Leave line commented if using DHCP
//#define WIFI_USE_STATIC_IP
// Wifi private static IP address (if WIFI_USE_STATIC_IP is defined)
#define WIFI_IP IPAddress(192,168,1,2)
// Wifi Gateway (if WIFI_USE_STATIC_IP is defined)
#define WIFI_GATEWAY IPAddress(192,168,1,1)
// Wifi Subnet (if WIFI_USE_STATIC_IP is defined)
#define WIFI_SUBNET IPAddress(255,255,255,0)
// Wifi name of the station
#define WIFI_NAME String("ESP8266-NAME")

// Private constants for Thingspeak channel
// ID of the channel
#define IOT_CHANNEL_ID "00000"
// Key of the stream
#define IOT_STREAM_KEY String("0000000000000000")

// Private constants for the Schedule on GIST
// URL of the schedule
#define SCHEDULE_URL "/<github-user>/<gist-id>/raw/<gist-file>"
#endif
