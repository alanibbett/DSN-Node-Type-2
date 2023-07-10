void mountFileSystem(){

if (!SD.begin()) { 
     display.println("Card failed, or not present");  // Print a message if the SD card
      display.setTextColor(RED); 
      display.println("reload and restart...");        // initialization fails or if the
      while (1);                                      // SD card does not exit
    }
    else {
      display.println("TF card initialized."); 
      display.println("ls >"); 
      File root = SD.open("/");
     // printDirectory(root, 1);    
    }





} // mount file system


void printDirectory(File dir, int numTabs) {
  
   while (true) {
      
      File entry = dir.openNextFile();
      if (! entry) {
         // no more files
         break;
      }
      if (entry.name()[1] =='.') continue;
      
      for (uint8_t i = 0; i < numTabs; i++) {
         display.print(" ");
      }
      display.print(entry.name());
      if (entry.isDirectory()) {
         display.println("/");
         printDirectory(entry, numTabs + 1);
      } else {
         // files have sizes, directories do not
         display.printf("  %d\n\r",entry.size());
      }
      entry.close();
   }
   
}// printDirectory

char* getFlashCardType(){

   uint8_t cardType = SD.cardType();
   switch (cardType) {
   case CARD_NONE:
        return "No Card Present";
        break; 
   case CARD_MMC:
        return "MMC";
        break;
   case CARD_SD:
        return "SDSC";
        break;
   case CARD_SDHC:
        return "SDHC";
        break;
   default:
        return "Unknown";
   
   }
  
}

void showSDDetails() {
  uint64_t cardSize = SD.cardSize()/(1024*1024);
  uint64_t totalBytes = SD.totalBytes()/(1024*1024);
  uint64_t totalUsed = SD.usedBytes()/(1024*1024);
  float percentUsed = (totalUsed/totalBytes)*100;
  
  // Display free space
  display.println("Flash Card Details:");
  display.print("Card Type:");display.println(getFlashCardType());
  display.printf("Card size %11uMB\n",cardSize);
  display.printf("Total space %11uMB\n",totalBytes);
  display.printf("Total used  %11uMB\n",totalUsed);
  display.printf("Used %6.2f%%\n",totalBytes);
}
