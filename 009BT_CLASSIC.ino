
void btClassicSetup() {
  SerialBT.begin("DSN Node1"); //Bluetooth device name
  Serial.println("Bluetooth Classic Started"); 
}

void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
  char sql[255]; // string to hold the database query
  //snprintf(sql,255,"insert into bt_stations (bssid,class,name,latitude,longitude,date) values('%s','%u','%s','%d','%d','%d');",
  snprintf(sql,255,"insert into bt_stations (bssid,class,name,latitude,longitude,date,RSSI) values('%s','%u','%s','%-.6f','%-.6f','%d','%d');",
          pDevice->getAddress().toString().c_str(),
          pDevice->getCOD(),
          pDevice->getName().c_str(),
          latitude,
          longitude,  
          now(),
          pDevice->getRSSI()); //snprintf to create SQL
          // Now actually log the data
          logProbeData(sql);
  
  
      if (printBTCScan){
          Serial.printf("Device name %s Class %u RSSI %i  Address %s\r\n",
          pDevice->getName().c_str(),
          pDevice->getCOD(),
          pDevice->getRSSI(),
          pDevice->getAddress().toString().c_str()
          );
        } // print to console
}// call back for discovered bt device







void btRunScan (){


  if (btScanAsync){
    //Serial.print("Starting discoverAsync...");
    if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
        delay(10000);
        SerialBT.discoverAsyncStop();
    } else {
        Serial.println("Error on discoverAsync i.e. not working after a \"connect\"");
    }
  }

  if (btScanSync) {
    
    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
    uint16_t devsfound = pResults->getCount();
    if (printBTCScan){
        Serial.println("Starting BT discovery ...");
        Serial.printf("%i devices found \r\n",devsfound);
    } //if print
    if (devsfound > 0){
      for (int i=0;i<devsfound;i++){
        // log the found devices to the database
        char sql[255]; // string to hold the database query
        BaseType_t qStatus;
       // Now build up the SQL query
       
        snprintf(sql,255,"insert into bt_stations (bssid,class,name,latitude,longitude,date,RSSI) values('%s','%u','%s','%-.6f','%-.6f','%d','%d');",
          pResults->getDevice(i)->getAddress().toString().c_str(),
          pResults->getDevice(i)->getCOD(),
          pResults->getDevice(i)->getName().c_str(),
          latitude,
          longitude,  
          now(),
          pResults->getDevice(i)->getRSSI()); //snprintf to create SQL

          
          // put it on the queue to save to database
            log_d("SQL Query is: %s", sql);
          // actually log the data for this device
            //btQueries(sql);
            logProbeData(sql);
          // check to see if we need to print this to the console       
          if (printBTCScan){
              Serial.printf("Device name %s Class %u RSSI %i  Address %s\r\n",
              pResults->getDevice(i)->getName().c_str(),
              pResults->getDevice(i)->getCOD(),
              pResults->getDevice(i)->getRSSI(),
              pResults->getDevice(i)->getAddress().toString().c_str()
            );// print btscan
        }// if print to console
      } //iterate through devices loop
      
    } // if some results to print (devices > 0)
 }// if BT sync scan enabled
} // bt run scan


/*
void blcScanloop(void *pvParameters) {
  (void) pvParameters;
  Serial.println("Bluetooth Classic Scan task Started.");
  for (;;){
  btRunScan();
  blcScanloopHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // record the max memory used in this task
  vTaskDelay(15000/ portTICK_PERIOD_MS);
  } // loop and don't end
}
*/
