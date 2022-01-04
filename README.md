# ClockProject
This is the GitHub Companion to the Large scale LED Clock as shown in https://youtu.be/gxohO7Plrmg
![Thumbnail](https://github.com/Zindswini/ClockProject/blob/v1.0/images/thumbnail.jpg?raw=true) 
![TablePicture](https://github.com/Zindswini/ClockProject/blob/v1.0/images/TablePicture.png?raw=true) ![Fusion Render](https://github.com/Zindswini/ClockProject/blob/v1.0/images/fusionrender.png?raw=true)

# Design Files
- 3D work was done in Autodesk Fusion 360, and files are provided as F3Z and STL files.
- Circuit Design was done in Autodesk Eagle, files are provided as Board and Schematic files.
- Manufacturing was done through JLCPCB, and Gerber files are provided.

# Arduino Dependencies
Unfortunately I cannot give precompiled binaries, as you need to include wifi information.

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
