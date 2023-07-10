void TaskGUI(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  Serial.println("GUI task Started.");

  
  for (;;) // A Task shall never return or exit.
  {   
        handleButtonPushes();
        
         switch (screenNumber) {

          case 0:   
                  drawScreenOne();
                  break;
          case 1: 
                  drawScreenTwo();
                  break;
          case 2:

                  drawScreenThree();
                  break;
          case 3:
                  drawScreenFour();
                  break;
          case 4:
                 
                  drawScreenFive();
                  break;
          
         }
         
         updateNodeSettings();
         vTaskDelay(500/ portTICK_PERIOD_MS);
         changeWifiChannel();
         guiHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); //find the high watermark for the guitask
         sCmd.readSerial();     // We don't do much, just process serial commands
         M5.update();
  }
}



void setUpDisplay(){

    
    
    uint16_t boxHeight = 200;
    display.begin();
    display.fillScreen(TFT_BLACK);
    display.drawCenterString("DSN STARTUP",display.width()/2,10,&fonts::FreeSansBold9pt7b);
    display.setFont(&fonts::Font2);
    createTextBox(&display,0,display.height()-boxHeight,display.width(),boxHeight);
    //drawSplash("/sCSU_emblem.png");

}

void handleButtonPushes(){

      if (M5.BtnB.wasPressed()){
          incrementScreenNumber();
       }
    if (screenNumber==0) {
      // only do stuff on the first screen
      if (M5.BtnA.wasPressed()){
        wifiScanSync = !wifiScanSync;
      }
      if (M5.BtnC.wasPressed()){
        btScanAsync = !btScanAsync;
      }

    }
  
}



void drawSplash(char* pic){
      display.drawPngFile(SD,pic,136,0);
}


void  placeItem (M5Canvas* disp,char* titleTxt, char* valueTxt, uint8_t line){
         uint8_t lineHeight = 16;
         uint8_t rowStart   = 40; // the y value for the top of the first row
         uint8_t lineStart = 30;  // the x value for the item title
         uint8_t dataStart = 130; // the x value for the data title
         disp->setCursor(lineStart, rowStart + (line*lineHeight));
         disp->printf(titleTxt);
         disp->setCursor(dataStart, rowStart +  (line*lineHeight));
         disp->printf(valueTxt);
}


void drawScreenOne(){
  
        char values[80]; // a buffer to hold the values to be displayed
        display.clearScrollRect(); // turn off any scroll boxes
        
        mainScreen.fillSprite(TFT_BLACK);
        mainScreen.drawRoundRect(0, 0, 320, 240, 10, TFT_YELLOW);
        mainScreen.drawCenterString((String) thisNode.name,display.width()/2,10,&fonts::FreeSansBold9pt7b);
        mainScreen.setFont(&fonts::Font2);
        mainScreen.setCursor(25, 25);
        mainScreen.setTextColor(TFT_YELLOW); 
        mainScreen.printf("General Statistics");
        mainScreen.setTextColor(TFT_WHITE); 
        
         // Place titles on the left of the screen

        sprintf (values,"%i of %i",screenNumber+1,maxscreenNumber);
        placeItem(&mainScreen,"Screen",values,0);
        sprintf(values,"%s %02d-%02d-%04d",getDayOfWeek(weekday()),day(), month(),year() );
        placeItem(&mainScreen,"Date/Time",values,1);
        sprintf(values,"%02d-%02d-%02d",hour(),minute(),second());
        placeItem(&mainScreen,"",values,2);
        sprintf(values,"%d seconds",thisNode.runTime);
        placeItem(&mainScreen,"Run Time",values,3);
        sprintf(values,"%s %02d-%02d-%04d",getDayOfWeek(weekday(thisNode.lastBoot)),day(thisNode.lastBoot), month(thisNode.lastBoot),year(thisNode.lastBoot) );
        placeItem(&mainScreen,"Last Reboot",values,4);
        sprintf(values,"%02d-%02d-%02d",hour(thisNode.lastBoot),minute(thisNode.lastBoot),second(thisNode.lastBoot));
        placeItem(&mainScreen,"",values,5);
        sprintf (values,"%s",codeVer);
        placeItem(&mainScreen,"Firmware",values,6);
        sprintf (values,"%s",(btScanAsync)?"Yes":"No");
        placeItem(&mainScreen,"Scanning BT",values,8);
        sprintf (values,"%s",(wifiScanSync)?"Yes":"No");
        placeItem(&mainScreen,"Scanning wifi",values,9);

        
        mainScreen.setCursor(30, 215);
        mainScreen.printf("%s",(wifiScanSync)?"stop wifi":"start wifi");
        mainScreen.setCursor(235, 215);
        mainScreen.printf("%s",(btScanAsync)?"stop BT":"start BT");
          
         mainScreen.pushSprite(0, 0);// Push the Sprite to the screen

  
}// draw screen 1



void drawScreenTwo(){

        char values[80]; // a buffer to hold the values to be displayed
        display.clearScrollRect();                                   // turn off any scroll boxes 
        mainScreen.fillSprite(TFT_BLACK);                            // cler screen  
        mainScreen.drawRoundRect(0, 0, 320, 240, 10, TFT_ORANGE);     // draw a nice bounding box
        
        mainScreen.drawCenterString((String) thisNode.name,display.width()/2,10,&fonts::FreeSansBold9pt7b);
        mainScreen.setFont(&fonts::Font2);
        mainScreen.setCursor(25, 25);
        mainScreen.setTextColor(TFT_ORANGE); 
        mainScreen.printf("RTOS Statistics");
        mainScreen.setTextColor(TFT_WHITE); 
        
        // Place titles on the left of the screen and the values on the right

        
        sprintf (values,"%i of %i",screenNumber+1,maxscreenNumber);
        placeItem(&mainScreen,"Screen",values,0);
        sprintf (values,"%i running",uxTaskGetNumberOfTasks());
        placeItem(&mainScreen,"# Tasks:",values,1);
        
        placeItem(&mainScreen,"Task Stack High Water Mark","",2);
        placeItem(&mainScreen,"    (close to zero is bad)","",3);
        sprintf (values,"%i",guiHighWaterMark);
        placeItem(&mainScreen," - GUI",values,4);
        sprintf (values,"%i",databaseloopHighWaterMark);
        placeItem(&mainScreen," - Database",values,5);         
        sprintf (values,"%i",gpsloopHighWaterMark);
        placeItem(&mainScreen," - GPS",values,6);



          
         mainScreen.pushSprite(0, 0);// Push the Sprite to the screen

  
}// draw screen 2



void drawScreenThree(){

        char values[80]; // a buffer to hold the values to be displayed
        display.clearScrollRect();                                   // turn off any scroll boxes 
        mainScreen.fillSprite(TFT_BLACK);                            // cler screen  
        mainScreen.drawRoundRect(0, 0, 320, 240, 10, TFT_GREEN);     // draw a nice bounding box
        
        mainScreen.drawCenterString((String) thisNode.name,display.width()/2,10,&fonts::FreeSansBold9pt7b);
        mainScreen.setFont(&fonts::Font2);
        
        mainScreen.setCursor(25, 25);
        mainScreen.setTextColor(TFT_GREEN);
        mainScreen.printf("Power Supply Details");
        mainScreen.setTextColor(TFT_WHITE); 
        
        // Place titles on the left of the screen and the values on the right

        
        sprintf (values,"%i of %i",screenNumber+1,maxscreenNumber);
        placeItem(&mainScreen,"Screen",values,0);
        sprintf(values,"%3.1f V",thisNode.battVolts);
        placeItem(&mainScreen,"Battery",values,2);
        sprintf(values,"%-6.2f mW",thisNode.battPower);
        placeItem(&mainScreen,"Battery Power",values,3);
        sprintf (values,"%s",(M5.Axp.isCharging())?"Charging":"Not Charging");
        placeItem(&mainScreen,"Battery",values,4);



          
         mainScreen.pushSprite(0, 0);// Push the Sprite to the screen

  
}// draw screen 3




void drawScreenFour(){

        char values[80]; // a buffer to hold the values to be displayed
        display.clearScrollRect();                                   // turn off any scroll boxes 
        mainScreen.fillSprite(TFT_BLACK);                            // cler screen  
        mainScreen.drawRoundRect(0, 0, 320, 240, 10, TFT_BLUE);     // draw a nice bounding box
        
        mainScreen.drawCenterString((String) thisNode.name,display.width()/2,10,&fonts::FreeSansBold9pt7b);
        mainScreen.setFont(&fonts::Font2);
        
        mainScreen.setCursor(25, 25);
        mainScreen.setTextColor(TFT_BLUE);
        mainScreen.printf("GPS Status");
        mainScreen.setTextColor(TFT_WHITE); 
        
        // Place titles on the left of the screen and the values on the right
        sprintf (values,"%i of %i",screenNumber+1,maxscreenNumber);
        placeItem(&mainScreen,"Screen",values,0);
        
        sprintf (values,"%s",(gpsLockedOK())?"Locked":"No Lock");
        placeItem(&mainScreen,"gps lock",values,2);
        sprintf(values,"%-.6f",latitude);
        placeItem(&mainScreen,"Latitude",values,3);
        sprintf(values,"%-.6f",longitude);
        placeItem(&mainScreen,"Longitude",values,4);
        sprintf(values,"%d",gps.charsProcessed());
        placeItem(&mainScreen,"Processed",values,5);
        sprintf(values,"%d",gps.failedChecksum());
        placeItem(&mainScreen,"Errors",values,6);
        sprintf(values,"%d",gps.satellites.value());
        placeItem(&mainScreen,"# Satellites",values,7);

          
         mainScreen.pushSprite(0, 0);// Push the Sprite to the screen

  
}// draw screen 4


void drawScreenFive(){

    // future

  
} // draw screen 5

void createTextBox(M5GFX* disp, uint16_t startx, uint16_t starty, uint16_t boxwidth, uint16_t boxheight){
    uint16_t pad = 4;
    disp->setTextScroll(true);   
    disp->drawRect(startx,starty,boxwidth,boxheight,WHITE);
    disp->setScrollRect(startx + pad, starty+pad, boxwidth-pad-pad , boxheight-pad-pad);
    

  
}



/*
 * callback for a right swipe event
 * note the M5Stack Doc is out of date
 */
void swRight(Event& e) {
  incrementScreenNumber();
  //Serial.printf("Swipe Right Detected, Screen %d showing\n\r",screenNumber+1);
}

/*
 * callback for a left swipe event
 * note the M5Stack Doc is out of date
 */
void swLeft(Event &e) {
  decrementScreenNumber();
  //Serial.printf("Swipe Left Detected, Screen %d showing\n\r",screenNumber+1);

  
}

/*
 * increment the screen number to show
 * on the LCD roll over at zero and maxscreenNumber
 */
void incrementScreenNumber(){
    screenNumber++;
    if (screenNumber > maxscreenNumber-1) {
      screenNumber = 0;
    }
}


/*
 * Decrement the screen number to show
 * on the LCD roll over at zero and maxscreenNumber
 */

void decrementScreenNumber(){
    
    screenNumber--;
    if (screenNumber < 0) {
      screenNumber = maxscreenNumber;
    }
}
