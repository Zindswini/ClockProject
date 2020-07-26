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
const char* openweathermapid = "4718711"; // Banska Bystrica
const char* openweathermapq = "pflugerville"; // Banska Bystrica
const char* openweathermapunits = "metric"; // Celsius

#define twelvehr false
#define timePer 10 //the time per screen
#define fadeTime 0.3 //the total fade time
#define lowAmbientBrightness 70

//deifne timezone, referenced from UTC
const long utcOffsetInSeconds = 0;
const int utcOffsetInHours = -5; //utc

//Not important
String param = String(url) +
               "?q=" + openweathermapq +
               "&units=" + openweathermapunits +
               "&APPID=" + weatherKey;
String line;
