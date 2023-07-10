/*
 * Generic formatting and translation routines
 * 
d88888b  .d88b.  d8888b. .88b  d88.  .d8b.  d888888b d888888b d888888b d8b   db  d888b  
88'     .8P  Y8. 88  `8D 88'YbdP`88 d8' `8b `~~88~~' `~~88~~'   `88'   888o  88 88' Y8b 
88ooo   88    88 88oobY' 88  88  88 88ooo88    88       88       88    88V8o 88 88      
88~~~   88    88 88`8b   88  88  88 88~~~88    88       88       88    88 V8o88 88  ooo 
88      `8b  d8' 88 `88. 88  88  88 88   88    88       88      .88.   88  V888 88. ~8~ 
YP       `Y88P'  88   YD YP  YP  YP YP   YP    YP       YP    Y888888P VP   V8P  Y888P
  
 * 
 */

// Returns the day of the week as a C string
char* getDayOfWeek(uint8_t day){
   switch (day) {
   case 1:
        return "Sun";
        break; 
   case 2:
        return "Mon";
        break;
   case 3:
        return "Tue";
        break;
   case 4:
        return "Wed";
        break;
  case 5:
        return "Thu";
        break; 
  case 6:
        return "Fri";
        break;
  case 7:
        return "Sat";
        break; 
   default:
        return "DrWho";
   
   }
  
}


//Returns the reason the device was reset for as a string
char* printResetReason(int reason)
{
  switch ( reason)
  {
    case 1 : return "POWERON_RESET";break;          /**<1,  Vbat power on reset*/
    case 3 : return "SW_RESET";break;               /**<3,  Software reset digital core*/
    case 4 : return "OWDT_RESET";break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : return "DEEPSLEEP_RESET";break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : return "SDIO_RESET";break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : return "TG0WDT_SYS_RESET";break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : return "TG1WDT_SYS_RESET";break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : return "RTCWDT_SYS_RESET";break;       /**<9,  RTC Watch dog Reset digital core*/
    case 10 : return "INTRUSION_RESET";break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : return "TGWDT_CPU_RESET";break;       /**<11, Time Group reset CPU*/
    case 12 : return "SW_CPU_RESET";break;          /**<12, Software reset CPU*/
    case 13 : return "RTCWDT_CPU_RESET";break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : return "EXT_CPU_RESET";break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : return "RTCWDT_BROWN_OUT_RESET";break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : return "RTCWDT_RTC_RESET";break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : return "NO_MEAN";
  }
}
//Returns more verbose version of the reason the device was reset for as a string
void verbose_print_reset_reason(int reason)
{
  switch ( reason)
  {
    case 1  : Serial.println ("Vbat power on reset");break;
    case 3  : Serial.println ("Software reset digital core");break;
    case 4  : Serial.println ("Legacy watch dog reset digital core");break;
    case 5  : Serial.println ("Deep Sleep reset digital core");break;
    case 6  : Serial.println ("Reset by SLC module, reset digital core");break;
    case 7  : Serial.println ("Timer Group0 Watch dog reset digital core");break;
    case 8  : Serial.println ("Timer Group1 Watch dog reset digital core");break;
    case 9  : Serial.println ("RTC Watch dog Reset digital core");break;
    case 10 : Serial.println ("Instrusion tested to reset CPU");break;
    case 11 : Serial.println ("Time Group reset CPU");break;
    case 12 : Serial.println ("Software reset CPU");break;
    case 13 : Serial.println ("RTC Watch dog Reset CPU");break;
    case 14 : Serial.println ("for APP CPU, reseted by PRO CPU");break;
    case 15 : Serial.println ("Reset when the vdd voltage is not stable");break;
    case 16 : Serial.println ("RTC Watch dog reset digital core and rtc module");break;
    default : Serial.println ("NO_MEAN");
  }
}



/* 
 * Routines aimed at the Serial Cosole
 .o88b.  .d88b.  d8b   db .d8888.  .d88b.  db      d88888b 
d8P  Y8 .8P  Y8. 888o  88 88'  YP .8P  Y8. 88      88'     
8P      88    88 88V8o 88 `8bo.   88    88 88      88ooooo 
8b      88    88 88 V8o88   `Y8b. 88    88 88      88~~~~~ 
Y8b  d8 `8b  d8' 88  V888 db   8D `8b  d8' 88booo. 88.     
 `Y88P'  `Y88P'  VP   V8P `8888Y'  `Y88P'  Y88888P Y88888P
 * 
 * 
 * 
 * 
 */
// Prints ready message to console
void readyToGo(){
    
    Serial.println("************ Ready to go ************");
    getHelp();
    prompt("Cmd ");
}

// for VT100 terminals clear and home the screen
void clearAndHome()
{
  for (int i=0;i<30;i++){
    Serial.println(); // send a bunch of chraracters out to clear the screen
  }
  char esc=27;
  Serial.print(esc); // ESC
  Serial.print("[2J"); // clear screen
  Serial.print(esc); // ESC
  Serial.print("[H"); // cursor to home
  
}

// prints a directory listing to the console
void dir() {
  prompt("dir");
  if (SD.begin()){
      File root = SD.open("/");
      listFlash (root,3);
      prompt("dir ok");
  } else {
    Serial.println("Check Flash Card");
  }
}

// prints a directory listing to the console
void prompt(char* prompt){
   Serial.printf("%s>\n\r",prompt);
  
}

// prints a spacer line  to the Serial terminal
void serialPrintSpacer (char *message){
  Serial.printf("******************************** %s ******************************** \n\r",message);
  
}


/*
 * Routines aimed at the LCD Cosole
db       .o88b. d8888b. 
88      d8P  Y8 88  `8D 
88      8P      88   88 
88      8b      88   88 
88booo. Y8b  d8 88  .8D 
Y88888P  `Y88P' Y8888D' 
 * 
 * 
 */

// prints a spacer line  to the LCD
void printSpacer (char *message){
  display.setTextColor(GREEN);
  display.printf("*** %s ***\n\r",message);
  display.setTextColor(WHITE);
}


// prints the amount of free memory the LCD
void printFreeMemory(){
  display.println("Memory Details:");
  display.printf("Heap size: %d\n", ESP.getHeapSize());
  display.printf("Free Heap: %d\n", esp_get_free_heap_size());
  display.printf("Min Free Heap: %d\n", esp_get_minimum_free_heap_size());
  display.printf("Largest Free block: %d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
        
}
