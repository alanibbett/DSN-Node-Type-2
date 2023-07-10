

/*
 * 
d888888b .88b  d88. d8888b.  .d88b.  d8888b. d888888b .d8888. 
  `88'   88'YbdP`88 88  `8D .8P  Y8. 88  `8D `~~88~~' 88'  YP 
   88    88  88  88 88oodD' 88    88 88oobY'    88    `8bo.   
   88    88  88  88 88~~~   88    88 88`8b      88      `Y8b. 
  .88.   88  88  88 88      `8b  d8' 88 `88.    88    db   8D 
Y888888P YP  YP  YP 88       `Y88P'  88   YD    YP    `8888Y'
 * 
 * 
 * 
 * 
 */

//**** Basic Headers
#include <M5Core2.h>
#include <M5GFX.h>
#include <sqlite3.h>
#include "freertos/FreeRTOS.h"
#include <TimeLib.h>
#include <Time.h>
#include <SerialCommand.h>
#include <IniFile.h>
//**** WiFi Headers
#include "esp_wifi.h"
#include "esp_wifi_types.h"
//**** Bluetooth Headers
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BluetoothSerial.h>
//**** System Headers
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
//**** GPS Headers
#include <TinyGPS++.h>
#include <rom/rtc.h> // for get reset reason 

/*
 * 
 d8888b. d88888b d88888b d888888b d8b   db d88888b .d8888. 
88  `8D 88'     88'       `88'   888o  88 88'     88'  YP 
88   88 88ooooo 88ooo      88    88V8o 88 88ooooo `8bo.   
88   88 88~~~~~ 88~~~      88    88 V8o88 88~~~~~   `Y8b. 
88  .8D 88.     88        .88.   88  V888 88.     db   8D 
Y8888D' Y88888P YP      Y888888P VP   V8P Y88888P `8888Y'
 * 
 * 
 * 
 * 
 */


//Set up WiFi Sniffer Code
#define                       WIFI_CHANNEL_MAX  (13)
// Bluetooth classic parameters
#define                       BT_DISCOVER_TIME  10000


/*
 * 
 * 
 .d8888. d888888b d8888b. db    db  .o88b. d888888b .d8888. 
88'  YP `~~88~~' 88  `8D 88    88 d8P  Y8 `~~88~~' 88'  YP 
`8bo.      88    88oobY' 88    88 8P         88    `8bo.   
  `Y8b.    88    88`8b   88    88 8b         88      `Y8b. 
db   8D    88    88 `88. 88b  d88 Y8b  d8    88    db   8D 
`8888Y'    YP    88   YD ~Y8888P'  `Y88P'    YP    `8888Y' 
 * 
 * 
 * 
 * 
 */
// create structure to hold node properties that will be reported on later
struct nodeDetails {
        char      name[24];
        float     battVolts;
        float     battPower;
        
        uint64_t  diskFree;
        time_t    lastBoot;
        time_t    runTime;
        bool      scanWifi;
        bool      scanBT;
        bool      debugMe; 
        bool      isCharging; 
};



/*
 * 
 * 
 .d88b.  d8888b.    d88b d88888b  .o88b. d888888b .d8888. 
.8P  Y8. 88  `8D    `8P' 88'     d8P  Y8 `~~88~~' 88'  YP 
88    88 88oooY'     88  88ooooo 8P         88    `8bo.   
88    88 88~~~b.     88  88~~~~~ 8b         88      `Y8b. 
`8b  d8' 88   8D db. 88  88.     Y8b  d8    88    db   8D 
 `Y88P'  Y8888P' Y8888P  Y88888P  `Y88P'    YP    `8888Y'
 * 
 * 
 * 
 * 
 */
// create all of the various system objects
sqlite3             *db1;                 // create a pointer to our database object
SerialCommand       sCmd;                 // The SerialCommand handler object
M5GFX               display;              // LCD Display object
M5Canvas            mainScreen(&display); // A virtual canvas to display stuff on
BluetoothSerial     SerialBT;             // Bluetooth Seial object used for the BLC scanning
TinyGPSPlus         gps;                        // GPS object

M5Canvas            logoScreen(&display); // A virtual canvas to display stuff on

/*
 * 
d88888b db    db d8b   db  .o88b. d888888b d888888b  .d88b.  d8b   db .d8888. 
88'     88    88 888o  88 d8P  Y8 `~~88~~'   `88'   .8P  Y8. 888o  88 88'  YP 
88ooo   88    88 88V8o 88 8P         88       88    88    88 88V8o 88 `8bo.   
88~~~   88    88 88 V8o88 8b         88       88    88    88 88 V8o88   `Y8b. 
88      88b  d88 88  V888 Y8b  d8    88      .88.   `8b  d8' 88  V888 db   8D 
YP      ~Y8888P' VP   V8P  `Y88P'    YP    Y888888P  `Y88P'  VP   V8P `8888Y'
 * 
 * 
 * 
 */
// Declare functions in other tabs (not strictly required but there you go)
void              mountFileSystem();
void              printSpacer();
void              setLcdConsole();
void              showSDDetails();
char*             getFlashCardType();
void              printFreeMemory();
// serial port commands
void              processSerialCommand();
void              unrecognizedCommand(const char *command);
void              printStatus();
void              serialCommandSetup();


/*
 * 
 d888b  db       .d88b.  d8888b.  .d8b.  db      .d8888. 
88' Y8b 88      .8P  Y8. 88  `8D d8' `8b 88      88'  YP 
88      88      88    88 88oooY' 88ooo88 88      `8bo.   
88  ooo 88      88    88 88~~~b. 88~~~88 88        `Y8b. 
88. ~8~ 88booo. `8b  d8' 88   8D 88   88 88booo. db   8D 
 Y888P  Y88888P  `Y88P'  Y8888P' YP   YP Y88888P `8888Y'
 * 
 * 
 * 
 * 
 */

struct                        nodeDetails thisNode;
char                          nodeName[]="Check UR Flash"; // a default name for a node
char                          codeVer[]="V2023.05.26.02";
const int                     consoleTxtSize = 1;
uint8_t                       level = 0, channel = 1;
static      wifi_country_t    wifi_country = {.cc="AU", .schan = 1, .nchan = 13}; //Most recent esp32 library struct
UBaseType_t                   guiHighWaterMark;
UBaseType_t                   databaseloopHighWaterMark;
UBaseType_t                   gpsloopHighWaterMark;
uint8_t                       scanTime = 5; //Duration in seconds for which to scan
static      bool              btScanSync = false;
static      bool              btScanAsync = false;
static      bool              wifiScanSync = false;
static      bool              databaseBusy = false;
uint8_t                       maxWifiPackets = 200;
int8_t                        screenNumber = 0;
uint8_t                       maxscreenNumber = 4;
uint16_t                      gpsLockFailTime = 1500;

uint8_t                       qHighWater = 0;
uint8_t                       packetCount = 0;
float                         latitude = 0;
float                         longitude = 0;
// Global flags
bool                          printWiFiScan = false;
bool                          printBTScan = false;
bool                          printBTCScan = false;
bool                          printGPSScan = false;
bool                          gpsLocked = false;


//ButtonColors onCol = {BLACK, WHITE, WHITE};
//ButtonColors offCol = {RED, WHITE, WHITE};
//Button myButton(10, 10, 200, 100, false, "I'm a button !", onCol, offCol);

void setup() {
/*
 * This function runs once and ses up the device ready to go
 * 
 * 
 * 
 */
//M5.BtnA.off = M5.BtnC.off = {BLUE, WHITE, NODRAW};
//M5.BtnA.on  = M5.BtnC.on = {RED, WHITE, WHITE};




//* Start Core Functionaliy
                M5.begin(); // start all of the M5 stack bits inclding lcd
                Serial2.begin(9600, SERIAL_8N1, 13, 14);
                setUpDisplay();
      printSpacer(" Core started ");
//* Start Onboard LCD 
                mainScreen.createSprite(320,240); // create a sprite as large as the main screen to draw stuff on.
                mainScreen.fillSprite(TFT_BLACK);
        
      printSpacer(" Display started ");
      printSpacer(" Console OK ");
//* Start Onboard time keeping by syncing with rtc 
                setupTime();
      printSpacer(" Date and Time Synced ");
//* Mount the File System and show some infomration from them
      printSpacer(" Mount FS ");
                mountFileSystem();
      printSpacer("");
                showSDDetails();
      printSpacer("");
//* Start the database then test it  
      printSpacer("Start Dbase");
                sqlite3_initialize();
                checkDatabase();
//* Set up the sniffers wifi and bluetooth              
      printSpacer("Setup WiFiSniffer");
                wifi_sniffer_init();
      printSpacer("Setup btClassic Scanner");
                btClassicSetup();
//* Set up the command parser
      printSpacer("Setup CLI");
                serialCommandSetup();
//* launch all tasks
      printSpacer("Launcing tasks");
                setupTasks();
                
//* Final preparations
                logDeviceData("Device Booted");
                establishNodeSettings();
                updateNodeSettings();
                clearAndHome();
                readyToGo();
}




void loop() {

  
  }
