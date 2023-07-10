void feedGPS(void *pvParameters){

   Serial.println("gps feeding started...");
  while (1){
    while (Serial2.available() > 0) gps.encode(Serial2.read());
    gpsloopHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); //find the high watermark for the datalogger task
    if (gpsLockedOK()){
      latitude = gps.location.lat();
      longitude = gps.location.lng();   
    }
    vTaskDelay(100/ portTICK_PERIOD_MS);
   }

  
}


bool gpsLockedOK(){
    if (gps.satellites.isValid()){
      if (gps.satellites.value() < 3){
        return false;
        } // data is valid but not enough satelites
      } else { // data is not even valid
        return false;
     }// else
     return true;
}  // gps locked
