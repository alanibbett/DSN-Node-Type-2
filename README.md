# DSN-Node-Type-2
## Installation Instructions
Arduino code for the DSN nodes based on the M5Stack Core 2 Series of devices. 
To install this code down load the entire repository and place them in a folder in your sketchbook called **DSN-Node-V4** (take note of the location and capitalisation). You will notice a number of tabs exist. The functions for a part of the code base are loacted in each of these tabs.

*  001SD_Card
*  002PrettyPrint
*  003DatabaseFunctions
*  004WiFiSniffer
*  005FreeRTOS
*  006GUI
*  007SerialCommand
*  008GPS
*  009BT_Classic
*  010Time_Functions
*  011StartupFunctions

## Libraries Required
Make sure you install the correct boards and libraries:

**Boards**

https://espressif.github.io/arduino-esp32/package_esp32_index.json
https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
(cut and paste these lines into your arduno setup for additional Board Manager URLs)

**Libraries**
* time (Paul Stoffregen)
* IniFile (Steve Marple)
* TinyGPSPlus (Mikal Hart)
* Sqlite3Esp32 (Arundale Ramanathan)
* SerialCommand (Alan Ibbett - download from Github and install in your libraries folder manually)


