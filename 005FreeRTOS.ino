void setupTasks(){
    // Now set up  tasks to run independently.
    BaseType_t xReturned;

    Serial.printf("Free Heap Size : %ukB\r\n",xPortGetFreeHeapSize()/1024);
    // Create the GUI Task.
    xReturned = xTaskCreatePinnedToCore(
          TaskGUI
          ,  "GUI control"   // A name just for humans
          ,  5120// This stack size can be checked & adjusted by reading the Stack Highwater
          ,  NULL
          ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
          ,  NULL 
           ,  tskNO_AFFINITY);

      if( xReturned == pdPASS )
        {
         Serial.println("GUI Task created ok");
          } else {
            Serial.println("*** Gui Task Creation Failed ***");
          }

        // Create the Datalogger Task.  
  xReturned = xTaskCreatePinnedToCore(
          taskDataLogger
          ,  "database logger"
          ,  2048 // Stack size
          ,  NULL
          ,  2 // Priority
          ,  NULL 
          ,  tskNO_AFFINITY);

         if( xReturned == pdPASS )
        {
         Serial.println("Datalogger Task created ok");
          } else {
            Serial.println("*** Datalogger Task Creation Failed ***");
          }       

        // Create the gps feeder Task.  
  xReturned = xTaskCreatePinnedToCore(
          feedGPS
          ,  "GPS Feeder"
          ,  1024 // Stack size
          ,  NULL
          ,  1 // Priority
          ,  NULL 
          ,  tskNO_AFFINITY);

         if( xReturned == pdPASS )
        {
         Serial.println("GPS Feeder Task created ok");
          } else {
            Serial.println("*** GPS Feeder Task Creation Failed ***");
          }       


  
    Serial.printf("Free Heap Size : %ukB\r\n",xPortGetFreeHeapSize()/1024);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.

  
}
