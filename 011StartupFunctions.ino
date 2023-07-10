// INI file error messages

void printErrorMessage(uint8_t e, bool eol = true)
{
  switch (e) {
  case IniFile::errorNoError:
    Serial.print("no error");
    break;
  case IniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case IniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case IniFile::errorBufferTooSmall:
    Serial.print("buffer too small");
    break;
  case IniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case IniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case IniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case IniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case IniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}




/*
 * 
 d8b   db  .d88b.  d8888b. d88888b 
888o  88 .8P  Y8. 88  `8D 88'     
88V8o 88 88    88 88   88 88ooooo 
88 V8o88 88    88 88   88 88~~~~~ 
88  V888 `8b  d8' 88  .8D 88.     
VP   V8P  `Y88P'  Y8888D' Y88888P
 * 
 *  nodeDetails 
        // fixed
        char      name[24];
        bool      scanWifi;
        bool      scanBT;
        bool      debugMe; 
        time_t    lastBoot;

        //variable
        float     battVolts;
        float     battPower; 
        bool      isCharging;   
        uint64_t  diskFree;        
        time_t    runTime;
         
 * 
 * 
 */

// Establish Node settings that don't change here
void establishNodeSettings(){
    // set up a buffer to get node settings from INI file (node.ini)
    const size_t bufferLen = 80;
    char buffer[bufferLen];
    const char *iniFile = "/node.ini";
    IniFile ini(iniFile);
    if (!ini.open()) {
        Serial.print("Ini file ");
        Serial.print(iniFile);
        Serial.println(" does not exist - you will need to fix it and reboot");
        // Cannot do anything else
        while (1);
  }
  Serial.println("Ini file found");

  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    printErrorMessage(ini.getError());
    Serial.println("You will need to fix it and reboot...");
    // Cannot do anything else
    while (1)
      ;
  }
    
// Get default values from the ini file
  if (ini.getValue("device", "name", buffer, bufferLen)) {
    strcpy(thisNode.name,buffer);
    ini.getValue("device", "latitude", buffer, bufferLen);
    latitude = atof(buffer);
    ini.getValue("device", "longitude", buffer, bufferLen);
    longitude = atof(buffer);
    ini.getValue("device", "logwifi", buffer, bufferLen, wifiScanSync);
    ini.getValue("device", "logbt", buffer, bufferLen, btScanAsync);
    Serial.printf("Name= %s, Latitude= %-.6f, Longitude= %-.6f \n\r",thisNode.name,latitude,longitude);
  }
  else {
    Serial.print("Could not read 'mac' from section 'network', error was ");
    printErrorMessage(ini.getError());
    strcpy(thisNode.name,nodeName);
  }

  
    thisNode.lastBoot = now();
    
    thisNode.scanWifi = false;
    thisNode.scanBT = false;
    thisNode.debugMe = false;

    
}


// Set Node settings that do change regularly here
void updateNodeSettings(){
    thisNode.battVolts = M5.Axp.GetBatVoltage();
    thisNode.battPower = M5.Axp.GetBatPower();
    thisNode.isCharging = M5.Axp.isCharging();
    thisNode.diskFree  = (SD.totalBytes()-SD.usedBytes())/(1024*1024);
    thisNode.runTime = now() - thisNode.lastBoot;
    


  
}
