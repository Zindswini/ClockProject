//define Wifi settings
#ifndef STASSID
#define STASSID "INSERT SSID"
#define STAPSK  "WIFI PASSWORD"
#define weatherKey  "OPENWEATHERMAP API KEY";
#endif

//OpenWeathermap configuration
const char* host = "api.openweathermap.org";
const int httpsPort = 443;
const char* url = "/data/2.5/weather";
const char* openweathermapid = "4718711";
const char* openweathermapq = "pflugerville";
const char* openweathermapunits = "metric"; //Don't touch

#define twelvehr false
#define timePer 10 //the time per screen
#define fadeTime 0.3 //the fade time
#define lowAmbientBrightness 70

//deifne timezone, referenced from UTC
const long utcOffsetInSeconds = 0; //Don't touch
const int utcOffsetInHours = -5; //utc

//Don't touch
String param = String(url) +
               "?q=" + openweathermapq +
               "&units=" + openweathermapunits +
               "&APPID=" + weatherKey;
String line;
