# DSN-Node-Type-2
## Installation Instructions
Arduino code for the DSN nodes based on the M5Stack Core 2 Series of devices. 
To install this code down load the entire repository and place them in a folder in your sketchbook called **DSN-Node-V4** (take note of the location and capitalisation). You will notice a number of tabs exist. The functions for a part of the code base are loacted in each of these tabs.

*  001SD_Card
  
         All of the SD card functions are in this tab.
 
*  002PrettyPrint

         This tab contains functions for printing out to the console.
           
*  003DatabaseFunctions

         This tab contains functions for manipulating the database.
    
*  004WiFiSniffer
*  05FreeRTOS
*  006GUI
*  007SerialCommand

         All of the commands that the node understands from the serial console.
   
*  008GPS
*  009BT_Classic
*  010Time_Functions
*  011StartupFunctions

## Libraries Required
Make sure you install the correct boards and libraries:

**Boards**

1. https://espressif.github.io/arduino-esp32/package_esp32_index.json
2. https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json

(cut and paste these lines into your arduno setup for additional Board Manager URLs)

**Libraries**
* time (Paul Stoffregen)
* IniFile (Steve Marple)
* TinyGPSPlus (Mikal Hart)
* Sqlite3Esp32 (Arundale Ramanathan)
* SerialCommand (Oringianlly by Stefan Rado modified by Alan Ibbett - download from aibbett Github and install in your libraries folder manually)

**Node Preparation and Compilation:**
Download the repository and install in your sketchbook folder. Then initialise the sd card with the files from the SD Card Content Folder. Make sure you set up the ini file as per the requirments for the node. It is the init file that creates unique nodes. Read the contents of the ini file for more details. Place the sdcard into the Core2 device.
The next step is to plug the Core2 into the PC and select the serial port in the Arduino environment. Now make sure you have the M5Stack Core 2 board selected in the arduino environment and compile to code. If it compiles ok then use the right arrow to compile and install it on the node. The node should be good to go.

## First Run Steps
The first time the node is run we must set the date and time. To do this open a serial terminal (could be the monitr window in the Arduino environment) and type
help then the enter key make sure it sends both (cr+lf).

This will bring up a menu of commands that the node understands.

The SETTIME command set the nodes time:
   >The settime command looks for 3 arguments Hours Minutes and Seconds, we do a check on them to make sure there are three and they are at least in valid ranges we barf if they are not. For example:

**SETTIME 07 45 00**

Similarly the SETDATE command sets the nodes date:

>the setdate command looks for 4 arguments weekday month, day and year, we do a check on them to make sure there are 4 and they are at least in valid ranges we barf if they are not. **REMEMBER Sunday is day 1** for example:

**SETDATE 1 2 3 2023** 

sets the date to Sunday the second of February 2023. **REMEMBER** the RTC does not know what day it is from the date so make sure the day of week matches the date or you will be very confused. (probably should fix this...)


