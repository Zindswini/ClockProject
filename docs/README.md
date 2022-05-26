# ClockProject V2.0
This is the GitHub Companion to the Large scale RGB LED Clock as shown in [VIDEO IN PROGRESS]
![Thumbnail](https://github.com/Zindswini/ClockProject/blob/v1.0/images/thumbnail.jpg?raw=true) 
![TablePicture](https://github.com/Zindswini/ClockProject/blob/v1.0/images/TablePicture.png?raw=true) ![Fusion Render](https://github.com/Zindswini/ClockProject/blob/v1.0/images/fusionrender.png?raw=true)

# Design Files
- 3D and Circuit design was completed using Autodesk Fusion 360. Exports from the project are provided.
- Manufacturing was completed through JLC PCB, Gerber files exported from Fusion 360 are provided.

# Dependencies & Building
This project uses PlatformIO to manage dependencies. In theory, this should mean that you will not have to worry about dependencies and versioning. Just in case, a list of used libraries is provided below.

In order to program your own clock, you need to provide the appropriate credentials for your Wifi network. In order to connect, update the STASSID and STAPSK fields.

This project relies on:
### Libraries:
- [Adafruit ESP8266](https://github.com/adafruit/Adafruit_ESP8266) (In Library Browser)
- [Arduino JSON](https://github.com/bblanchon/ArduinoJson/tree/5.x)  (In Library Browser) (*Note version 5.x*)
- [DHTesp](https://github.com/beegee-tokyo/DHTesp) (In Library Browser)
- [NTPClient](https://github.com/arduino-libraries/NTPClient) (In Library Browser)
- [TelnetStream](https://github.com/jandrassy/TelnetStream) (In Library Browser)

### ESP Board
Add http://arduino.esp8266.com/stable/package_esp8266com_index.json to the "Additional Board Manager URLs" in the Arduino IDE, then add the esp8266 from inside the board manager.

Be sure to select "Adafruit Feather HUZZAH ESP8266" when compiling.

(this might be obsolete now that the project uses PlatformIO)
