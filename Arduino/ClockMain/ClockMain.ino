#include <ArduinoJson.h>
#include <DHTesp.h>
#include <NTPClient.h>
#include <WiFiClientSecure.h>
#include "credentials.h"
#include "OTA.h"

//define I/O pins
#define DHT_PIN 16
#define LSESNSOR_PIN A0

#define DATAOUT 5
#define CLOCK 4
#define CLEAR 0
#define LATCH 2
#define ENABLE 15
#define TICK 13

//WORKING VARS
//init dht11 vars
double localHumidity;
double localTemperature;
double lastTemp;
DHTesp dht;

double outsideHumidity;
double outsideTemperature;

double timeOffset = 0.0;
int brightness;

//init wifi and ntp vars
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//init temp vars
int mode = 0;
bool switched = false;
bool fadeDir;

//init number definitions
byte numbs[12];

void setup() {
  pinMode(DATAOUT, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(CLEAR, OUTPUT);
  pinMode(LATCH, OUTPUT);
  //pinMode(TICK, OUTPUT);

  digitalWrite(CLEAR, HIGH);
  Serial.begin(115200);

  //Init services
  startWifi(); //takes a while
  timeClient.begin();
  timeClient.update();
  timeClient.setUpdateInterval(120);
  dht.setup(DHT_PIN, DHTesp::DHT11);

  //OTA
  ArduinoOTA.setHostname("Wall Clock");
  setupOTA();
  
  //Set initial register pins
  digitalWrite(CLEAR, HIGH);
  digitalWrite(ENABLE, LOW);

  //define Numbers
  numbs[0] =   0xFC;
  numbs[1] =   0x60;
  numbs[2] =   0xDA;
  numbs[3] =   0xF2;
  numbs[4] =   0x66;
  numbs[5] =   0xB6;
  numbs[6] =   0xBE;
  numbs[7] =   0xE0;
  numbs[8] =   0xFE;
  numbs[9] =   0xF6;
  numbs[10] =  0x51;
  numbs[11] = B00000000;
}

void loop() {
  //handle weather clock, brightness data updating
  if(timeClient.getSeconds() % 30 == 0 && switched == false) 
  {
    downloadData();

    //update time and offset
    long startTime = millis();
    timeClient.update();
    timeOffset = (millis() - startTime) / 1000;
    
    localTemperature = dht.getTemperature();
    localTemperature = constrain(localTemperature,0,999);

    localHumidity = dht.getHumidity();
    localHumidity = constrain(localHumidity,0,999);
  }

  //Handle mode switching
  if(timeClient.getSeconds() % timePer == 0)
  {
    if(switched == false){
      fade();
      switched = true;
    }
  }
  else if(switched == true) { switched = false; }
  
  //update brightness and tick
  brightness = updateBrightness();
  
  analogWrite(ENABLE, 1024 - updateBrightness());
  
  if(timeClient.getSeconds() % 2 == 0 && brightness == 1024){analogWrite(TICK, brightness);}
  else { analogWrite(TICK, 0); } 

  ArduinoOTA.handle();

  //shift out main display
  shifterOut(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(),mode);
  shiftMode(mode);
  pulse(LATCH);

  if(brightness < 1024){
    delay(500);
  }
  else{
  delay(50);
  }
  }

void shifterOut(int hr, int mn, int sec, int mode)
{
  //clear,shiftChar, latch
  switch (mode)
  {
    case 0: //Local time
      //calculate time difference
      int locHr;
      locHr = hr + utcOffsetInHours;
      if(locHr < 0){ locHr += 24; }
      else if(locHr > 24) { locHr -= 24; }
      
      //Shift hour, minute, second
      shiftNumber(locHr);
      shiftNumber(mn);
      shiftNumber(round(sec + timeOffset));
      //Serial.println(locHr);
      break;

    case 1: //UST time
      shiftNumber(hr);
      shiftNumber(mn);
      shiftNumber(round(sec + timeOffset));
      break;

    case 2: //Temp F
      shiftTemperature(round(convertToFahrenheit(localTemperature)),round(convertToFahrenheit(outsideTemperature)));
      break;

    case 3: //Temp C
      //shift outside temp
      shiftTemperature(round(localTemperature),round(outsideTemperature));
      break;

    case 4: //Humid %
      shiftTemperature(round(localHumidity),round(outsideHumidity));
      break;
  }
}

void startWifi()
{
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void pulse(int pin)
{
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void dip(int pin)
{
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
}

float convertToFahrenheit(float inTemp) { return inTemp * (9.00/5) + 32; }

String formatNumber(int in)
{
  if(in < 10) { return "0"+(String)in; }
  else { return (String)in; }
}

void shiftEmpty()
{
  digitalWrite(DATAOUT, LOW);
  for(int i = 0; i < 8; i++) { pulse(CLOCK); }
}
//Two digits max (duh)
void shiftNumber(int num)
{
  if(num < 10){
        shiftOut(DATAOUT,CLOCK, LSBFIRST,numbs[0]);
        shiftOut(DATAOUT,CLOCK, LSBFIRST,numbs[num]);
      }
      else{
        shiftOut(DATAOUT,CLOCK, LSBFIRST,numbs[num/10]);
        shiftOut(DATAOUT,CLOCK, LSBFIRST,numbs[num % 10]);
      }  
}

//properly handle single and triple digit temps
void shiftTemperature(int inTemp, int outTemp)
{
  inTemp = constrain(inTemp, 0, 999);
  outTemp = constrain(outTemp, 0, 999);
  
  if(outTemp <= 99) { shiftNumber(outTemp); shiftEmpty(); }
  else
  {
    shiftNumber(outTemp / 10);
    shiftOut(DATAOUT,CLOCK, LSBFIRST,numbs[outTemp % 10]);
  }
  
  if(inTemp <= 99) { shiftEmpty(); shiftNumber(inTemp);}
  else
  {
    shiftNumber(inTemp / 10);
    shiftOut(DATAOUT,CLOCK, LSBFIRST,numbs[inTemp % 10]);
  }
}

void shiftMode(int currentMode)
{
  digitalWrite(DATAOUT, HIGH);
  pulse(CLOCK);
  pulse(CLOCK);
  pulse(CLOCK);
  for(int i = 4; i >= 0; i--){
    if(currentMode == i)
    {
      digitalWrite(DATAOUT, HIGH);
    }
    else
    {
      digitalWrite(DATAOUT, LOW);
    }
    //digitalWrite(DATAOUT, HIGH);
    pulse(CLOCK);
  }
}

//thanks to RoboUlbricht on github for the demo code
void downloadData() {
  //connect to openweathermap
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET ") + param + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  line = client.readStringUntil('\n');
  if (line != "HTTP/1.1 200 OK\r") {
    Serial.print("Unexpected response: ");
    Serial.println(line);
    return;
  }

  if (client.find("\r\n\r\n")) {
    DynamicJsonBuffer jsonBuffer(4096);
    JsonObject& root = jsonBuffer.parseObject(client);
    
    outsideTemperature = root["main"]["temp"];
    Serial.print("Temperature: ");
    Serial.println(outsideTemperature);

    outsideHumidity = root["main"]["humidity"];
    Serial.print("Humidity: ");
    Serial.println(outsideHumidity);
  } 
}

int updateBrightness()
{
  int count = 0;
  int value = 0;

  //attmept "denoising" by taking many readings
  for(int i = 0; i < 1000; i++){
  value += analogRead(A0);
  }
  value /= 1000;
  value /= 2;
  TelnetStream.println(value);
  if(value < lowAmbientBrightness){ value = 1; }
  else { value = 512; }
  

  value *= 2;
  return value;
}

//fade out, update screen, fade in. 512 resolution due to small decimals.
void fade()
{
  if(brightness < 1024)
  {
    //change mode
    mode++;
    if(mode > 4) { mode=0; }
  
    shifterOut(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(),mode);
    shiftMode(mode);
    pulse(LATCH);
  }
  else{
  int fadebrightness = brightness / 2;
  //fade out
  while(fadebrightness > 0)
  {
    fadebrightness -= 1;
    analogWrite(ENABLE, 1024 - (fadebrightness * 2));
    delay(((fadeTime / 2) / brightness) * 10000);
    yield();
  }
  
  //change mode
  mode++;
  if(mode > 4) { mode=0; }

  //uncomment to disable UTC
  if(mode == 1) { mode++; }
  
  //uncomment to disable celsius
  if(mode == 3) { mode++; }

  //uncomment to disable humidity
  if(mode == 4) { mode++; }
  
  if(mode > 4) { mode=0; }
  
  shifterOut(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(),mode);
  shiftMode(mode);
  pulse(LATCH);
  
  
  //fade in
  while(fadebrightness < brightness / 2)
  {
    fadebrightness += 1;
    analogWrite(ENABLE, 1024 - (fadebrightness * 2));
    delay(((fadeTime / 2) / brightness) * 10000);
    yield();
  }
  }
  switched = false;
}
