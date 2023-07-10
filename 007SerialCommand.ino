/*
 * 
 * 
 .o88b. db      d888888b      d88888b db    db d8b   db  .o88b. d888888b d888888b  .d88b.  d8b   db .d8888. 
d8P  Y8 88        `88'        88'     88    88 888o  88 d8P  Y8 `~~88~~'   `88'   .8P  Y8. 888o  88 88'  YP 
8P      88         88         88ooo   88    88 88V8o 88 8P         88       88    88    88 88V8o 88 `8bo.   
8b      88         88         88~~~   88    88 88 V8o88 8b         88       88    88    88 88 V8o88   `Y8b. 
Y8b  d8 88booo.   .88.        88      88b  d88 88  V888 Y8b  d8    88      .88.   `8b  d8' 88  V888 db   8D 
 `Y88P' Y88888P Y888888P      YP      ~Y8888P' VP   V8P  `Y88P'    YP    Y888888P  `Y88P'  VP   V8P `8888Y'
 * 
 * 
 */


//*******************************************************
// Command to
// test various functions
void testMe(){
  qHighWater = 0;
  
}



//*******************************************************
// Command to
// Change the name of the Node
void renameNode(){
    char *arg;
    arg = sCmd.next();
      if (arg != NULL) {
          strcpy(thisNode.name,arg);
       } // null arg
    else {
      Serial.println("No arguments name not changed");
    }// if arg is not null  
} // rename node




//*******************************************************
// Command to
// Shutdown and power off the Node
void shutdownNode(){
    
  Serial.println("Shutting down now.....");
  logDeviceData("Manual Shutdown");
  sqlite3_close(db1);
  wifiScanSync = false;
  btScanSync = false;
  vTaskDelay(1000/ portTICK_PERIOD_MS);
  M5.shutdown();
  
}
//*******************************************************
// Command that
// enables console display of wifi scan results

void showWiFiScans(){
    char *arg;
    arg = sCmd.next();
    
      if (arg != NULL) {
        if (strcmp(arg,"ON")==0){
            Serial.println("displaying wifi scan results ");
            printWiFiScan = true;
            return;
        }// ARG = ON
       } // null arg
    else {
      Serial.println("No arguments - use on or off");
    }// if arg is not null
  printWiFiScan = false;
} // print wifiscans

//*******************************************************
// Command that
// enables console display of wifi scan results

void showBTScans(){
    char *arg;
    arg = sCmd.next();
      if (arg != NULL) {
        if (strcmp(arg,"ON")==0){
            Serial.println("displaying Bluetooth scan results ");
            printBTScan = true;
            return;
        }// ARG = ON
       } // null arg
    else {
      Serial.println("No arguments - use on or off");
    }// if arg is not null
  printBTScan = false;
} // print BTscans



//*******************************************************
// Command that
// enables console display of bluetooth classic scan results

void showBTCScans(){
    char *arg;
    arg = sCmd.next();
      if (arg != NULL) {
        if (strcmp(arg,"ON")==0){
            Serial.println("displaying Bluetooth classic scan results ");
            printBTCScan = true;
            return;
        }// ARG = ON
       } // null arg
    else {
      Serial.println("No arguments - use on or off");
    }// if arg is not null
  printBTCScan = false;
} // print BTCscans



//*******************************************************
// Command that
// switches wifi or bluetooth scanning on or off

void allowScans(){
    char *arg;
    char* argArray[2] = {"",""};
    uint8_t i=0;
    arg = sCmd.next();
    while (arg != NULL) {
          argArray[i] = arg;
          i++;
          arg = sCmd.next();
    } //while arg
    // check for BT controls
    if ( strcmp(argArray[0],"BT")==0){
      if (strcmp(argArray[1],"ON")==0){
          btScanAsync = true;
          Serial.println("BT classic async Scanning on.");
          return;  
        } // bt scanning on
      else {  btScanAsync = false;
              Serial.println("BT classic async Scanning off");
              return;
        } // bt scanning off
      } // check for BT setting
    // Check for wifi Controls
    if ( strcmp(argArray[0],"WIFI")==0){
      if (strcmp(argArray[1],"ON")==0){
          wifiScanSync = true; 
          Serial.printf("WIFI Scanning on. Channel %d\n\r", channel);   
          return;  
        } // wifi scanning on
      else {  wifiScanSync = false;
              Serial.println("WIFI Scanning off");
              return;
        } // wifi scanning off
      } // check for wifi setting

    
} // allow scans


//*******************************************************
// Command that
//prints the current status of the node
void  printStatus(){
    serialPrintSpacer("STATUS");
    prompt("status");
   
    Serial.println("Node Status:");
    Serial.printf("Time: %02d:%02d:%02d \r\n",hour(), minute(),second());
    Serial.printf("Date: %s %02d-%02d-%04d \r\n",getDayOfWeek(weekday()),day(),month(),year());
    serialPrintSpacer("OS Status");
    
    Serial.printf("OS is currently running %i tasks",uxTaskGetNumberOfTasks());Serial.println();
    Serial.print("Guitask highwatermark: ");Serial.print(guiHighWaterMark);Serial.println(" Close to 0 is bad");
    Serial.print("Database highwatermark: ");Serial.print(databaseloopHighWaterMark);Serial.println(" Close to 0 is bad"); 
    Serial.print("GPS highwatermark: ");Serial.print(gpsloopHighWaterMark);Serial.println(" Close to 0 is bad"); 
    serialPrintSpacer("CPU Status");  
    Serial.printf("CPU 0 reset reason %s\r\n",printResetReason(rtc_get_reset_reason(0)));
    Serial.printf("CPU 1 reset reason %s\r\n",printResetReason(rtc_get_reset_reason(1)));
    serialPrintSpacer("Database Recording Status");
    Serial.printf("WiFi scanning %s\r\n",(wifiScanSync)?"On":"Off");
    Serial.printf("BLC scanning %s\r\n",(btScanAsync)?"On":"Off");
    serialPrintSpacer("Printing Status");
    Serial.printf("Print WiFi scanning %s\r\n",(printWiFiScan)?"On":"Off");
    Serial.printf("Print BLC scanning %s\r\n",(printBTCScan)?"On":"Off");
    serialPrintSpacer("GPS Status");
    Serial.printf("*** Characters Processed:\t%-d *** Errors:\t%-d *** # Satellites:\t%-d ***\r\n",gps.charsProcessed(),gps.failedChecksum(),gps.satellites.value());
    //char rtStats[2048];
    //vTaskList( rtStats );
    prompt("status ok");
} // status

//*******************************************************
// Command that
// list the flash card contents to serial port
void listFlash (File dir, int numTabs) {
   while (true) {
      
      File entry = dir.openNextFile();
      if (! entry) {
         // no more files
         break;
      }
      if (entry.name()[1] =='.') continue;
      
      for (uint8_t i = 0; i < numTabs; i++) {
         Serial.print("  ");
      }
      Serial.print(entry.name());
      if (entry.isDirectory()) {
         Serial.println("/");
         listFlash(entry, numTabs + 1);
      } else {
         // files have sizes, directories do not
         Serial.print("  ");
         Serial.printf("(%-d) \n\r",entry.size());
      }
      entry.close();
   }//while
}// listFlash

//******************************************************* 
// CLI command to get clear the database
// does not compact the table (vacuum) or clear deviceLog table
void clearDatabase(){
         Serial.println ("Preparing to drop all data from database...");
  
                int rc;
                if (openDb("/sd/dsn.db", &db1)){
                       display.println("Cannot find database...");
                   return;}
         else {
                rc = db_exec(db1,"DELETE FROM bt_stations;DELETE FROM devices;DELETE FROM wifis;DELETE FROM stations;DELETE FROM probes;DELETE FROM gps;");
                if (rc != SQLITE_OK) {
                    sqlite3_close(db1);
                    Serial.println ("        all data dropped from database");
                }
                sqlite3_close(db1);
         }

  
}//clear database


//*******************************************************
//Command to 
//Set the time on the node RTC
void setNodeTime(){

  // the settime command looks for 3 arguments Hours Minutes and Seconds
  // we do a check on them to make sure there are three and they are at least
  // in valid ranges we barf if they are not.
  
  RTC_TimeTypeDef TimeStruct;
  int aNumber;
  char *arg;
  uint8_t argCount = 0;
  uint8_t argArray[3] = {0,0,0};
   
  Serial.println("We're in setTime");
  arg = sCmd.next();
  while (arg != NULL) {
      aNumber = atoi(arg);    // Converts a char string to an integer
      argArray[argCount] = aNumber;
      argCount++;
      arg = sCmd.next();
} //while arg
  // check for correct number of arguments
  if (argCount != 3) {
    // barf because not enough or too many parameters
    unrecognizedCommand("SETTIME: Needs exactly 3 arguments");
    return;
  }
  
  // check for valid Hours
  if ((argArray[0] < 0) || (argArray[0] >23)){
    unrecognizedCommand("SETTIME: Hours are between 0 and 23");
    return;
  }
 
  // check for valid Minutes
  if ((argArray[1] < 0) || (argArray[1] >59)){
    unrecognizedCommand("SETTIME: Minutes are between 0 and 59");
    return;
  }

  // check for valid Seconds
  if ((argArray[2] < 0) || (argArray[2] >59)){
    unrecognizedCommand("SETTIME: Seconds are between 0 and 59");
    return;
  }
  // to get to this point we have a valid array of time values now punch it in to the clock
  TimeStruct.Hours = argArray[0];
  TimeStruct.Minutes = argArray[1];
  TimeStruct.Seconds = argArray[2];
  M5.Rtc.SetTime(&TimeStruct);
  setupTime();
  prompt("time set ok");
}

//*******************************************************
//Command to 
//Set the date on the node RTC
void setNodeDate(){

  // the setdate command looks for 4 arguments weekday month, day and year
  // we do a check on them to make sure there are 4 and they are at least
  // in valid ranges we barf if they are not.
  
  RTC_DateTypeDef DateStruct;
  int aNumber;
  char *arg;
  uint8_t argCount = 0;
  uint16_t argArray[4] = {0,0,0};
   
  Serial.println("We're in setdate");
  Serial.println("Enter 4 numbers -> DAYofWeek(1-7) Month(1-12) Date(1-31) Year(yyyy)");
  arg = sCmd.next();
  while (arg != NULL) {
      aNumber = atoi(arg);    // Converts a char string to an integer
      argArray[argCount] = aNumber;
      argCount++;
      arg = sCmd.next();
}
  // check for correct number of arguments
  if (argCount != 4) {
    // barf because not enough or too many parameters
    unrecognizedCommand("SETDATE: Needs exactly 4 arguments");
    return;
  }
  
  // check for valid day of week
  if ((argArray[0] < 1) || (argArray[0] >7)){
    unrecognizedCommand("SETDATE: Day of week is between 1 (Sun) and 7 (Sat)");
    return;
  }
 
  // check for valid month
  if ((argArray[1] < 1) || (argArray[1] >12)){
    unrecognizedCommand("SETDATE: Months are between 1 and 12");
    return;
  }

  // check for valid day
  if ((argArray[2] < 1) || (argArray[2] >31)){
    unrecognizedCommand("SETDATE: Dates are between 0 and 31");
    return;
  }

    // check for valid year
  if ((argArray[3] < 2000) || (argArray[3] >2099)){
    unrecognizedCommand("SETDATE: Years are between 2000 and 2099");
    return;
  }
  // to get to this point we have a valid array of time values now punch it in to the clock
  DateStruct.WeekDay = argArray[0];
  DateStruct.Month = argArray[1];
  DateStruct.Date = argArray[2];
  DateStruct.Year = argArray[3];
  M5.Rtc.SetDate(&DateStruct);
  setupTime();
  prompt("date set ok");
}

//*******************************************************
//Command to 
//Show system Help
void getHelp(){
  // print a list of the CLI commands
  sCmd.getHelp();
  
}

//*******************************************************
//Command to 
//set as the default handler, and gets called when no other command matches.
void unrecognizedCommand(const char *command) {
  Serial.print(command);
  Serial.println(" Unrecognised or Invalid command - be better ");
}


/* 
 * All CLI COMMANDS ARE ADDED HERE
 *
 .o88b. db      d888888b      .d8888. d88888b d888888b db    db d8888b. 
d8P  Y8 88        `88'        88'  YP 88'     `~~88~~' 88    88 88  `8D 
8P      88         88         `8bo.   88ooooo    88    88    88 88oodD' 
8b      88         88           `Y8b. 88~~~~~    88    88    88 88~~~   
Y8b  d8 88booo.   .88.        db   8D 88.        88    88b  d88 88      
 `Y88P' Y88888P Y888888P      `8888Y' Y88888P    YP    ~Y8888P' 88   
 * 
 * 
 */
void serialCommandSetup() {

  // Setup callbacks for SerialCommand commands
  // Maximum command length 16 characters
  sCmd.addCommand("CLEAR",      readyToGo);                // This callback is in another file (prettyprint)
  sCmd.addCommand("DIR",        dir);                      // print list of files to the console
  sCmd.addCommand("DROP",       clearDatabase);           // clear entries from database
  sCmd.addCommand("SCAN",       allowScans);
  sCmd.addCommand("PRINTBT",    showBTScans);              // show or hide bluetooth scans on the monitor
  sCmd.addCommand("PRINTWIFI",  showWiFiScans);          // show or hide WiFi scans on the monitor
  sCmd.addCommand("PRINTBTC",   showBTCScans);          // show or hide WiFi scans on the monitor
  sCmd.addCommand("SHUTDOWN",   shutdownNode);          // Shutdown the Node (does it quickly)
  sCmd.addCommand("SETDATE",    setNodeDate);          // set RTC date
  sCmd.addCommand("SETTIME",    setNodeTime);          // set RTC time
  sCmd.addCommand("STATUS",     printStatus);          // Prints status
  sCmd.addCommand("RENAME",     renameNode);
  sCmd.addCommand("TEST",       testMe);
  sCmd.addCommand("HELP",       getHelp);                   // print list of commands
  sCmd.setDefaultHandler(unrecognizedCommand);        // Handler for command that isn't matched
  Serial.println("Ready");
}
