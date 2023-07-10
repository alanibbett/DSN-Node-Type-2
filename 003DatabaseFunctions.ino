
/*
 * 
.d8888.  .d88b.  db           d88888b db    db d8b   db  .o88b. d888888b d888888b  .d88b.  d8b   db .d8888. 
88'  YP .8P  Y8. 88           88'     88    88 888o  88 d8P  Y8 `~~88~~'   `88'   .8P  Y8. 888o  88 88'  YP 
`8bo.   88    88 88           88ooo   88    88 88V8o 88 8P         88       88    88    88 88V8o 88 `8bo.   
  `Y8b. 88    88 88           88~~~   88    88 88 V8o88 8b         88       88    88    88 88 V8o88   `Y8b. 
db   8D `8P  d8' 88booo.      88      88b  d88 88  V888 Y8b  d8    88      .88.   `8b  d8' 88  V888 db   8D 
`8888Y'  `Y88'Y8 Y88888P      YP      ~Y8888P' VP   V8P  `Y88P'    YP    Y888888P  `Y88P'  VP   V8P `8888Y' 
 * 
 */

//*******************************************************
// SQLite function to log probe data
// maybe this should be more general.....
void logProbeData(char* message){
            // the message contains the sql query 
            //Serial.printf("logging sql %s\n\r",message);
            vTaskDelay(100/ portTICK_PERIOD_MS);
            int rc;
            if (!databaseBusy){
              if (openDb("/sd/dsn.db", &db1)){
                  Serial.println("Cannot find database...");
                  return;              
              } else {  databaseBusy = true;
                        //Serial.println("logging sql");
                        rc = db_exec(db1,message);
                        if (rc != SQLITE_OK) {
                            sqlite3_close(db1);
                            databaseBusy =  false;
                         }// database write ok
                         sqlite3_close(db1);
                         databaseBusy =  false;
                      }// database opened ok
            } else {// if database not busy
            Serial.println("Database Busy");
            } // if database busy
  
} // log probe data




//*******************************************************
// SQLite error message handler
// gives a good deal of information on the error
char *zErrMsg = 0;
const char* data = "DSN node";


//*******************************************************
// SQLite handler to open database
// gives a good deal of information on any error
int openDb(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\nerror msg: %s\n",filename, sqlite3_errmsg(*db));
       display.printf("Can't open database: %s\nerror msg: %s\n",filename, sqlite3_errmsg(*db));
       return rc;
   } 
   return rc;
}



//*******************************************************
// wrapper function for handling the actual querry
// 
int db_exec(sqlite3 *db, const char *sql) {
   vTaskDelay(200/ portTICK_PERIOD_MS);
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg); // must free memory used for the error messages
   } 
   //Serial.println("Query OK");
   return rc;
}

//*******************************************************
// SQLite callback function that is returned by the db_exec when
// the query is completed. This one prints out the column name and values 
// for each returned row

static int callback(void *data, int argc, char **argv, char **azColName){   
/*   parameters explained:
          void*   data      - is a pointer to a parameter sent by dbexec
          int     argc      - a count of the row returned
          char**  arvg      - a pointer to an array of values returned
          char** azColName  - a pointer to the colum names in the return
 */
   int i;
   Serial.printf("%s: ", (const char*)data); // prints out the argument sent from dbExec
   
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n\r", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   //Serial.printf("\n");
   
   return 0;
}


//*******************************************************
// a simple utility function that opens the database file and executes
// a fixed query. returns 
void checkDatabase(){
       Serial.println("Check database");
       int rc;
       
        if (openDb("/sd/dsn.db", &db1)){
            display.println("Cannot find database...");
         return;}
         else
              {
                display.println("Database opened ok check console...");
                testQuery();
         }
  
}


//*******************************************************
// SQLite sample query
// 
void testQuery(){
                int rc;
                rc = db_exec(db1,"Select count(message) from deviceLog where message='Reboot'");
                if (rc != SQLITE_OK) {
                    sqlite3_close(db1);
                }
                sqlite3_close(db1);
}


//*******************************************************
// SQLite query to write to the log table
// also records the las reasons for the processors reboot
// and the time of the log entry

void logDeviceData(char* message){
            Serial.println("log device data");
            vTaskDelay(100/ portTICK_PERIOD_MS);
            char sql[255];
            int rc;
            if (openDb("/sd/dsn.db", &db1)){
              display.println("Cannot find database...");
              return;             
            } else {
                snprintf(sql,255,"insert into deviceLog (message,cIRebootReason,cIIRebootReason,time) values('%s','%s','%s','%d');",message,printResetReason(rtc_get_reset_reason(0)),printResetReason(rtc_get_reset_reason(1)),now());
                //return;        
                rc = db_exec(db1,sql);
                if (rc != SQLITE_OK) {
                    sqlite3_close(db1); //
                }
                sqlite3_close(db1); // 
                Serial.println("devicedata logged ok");
            }
  
}







void taskDataLogger(void *pvParameters)  // This is the main datalogging  task.
{

  Serial.println("database logging started...");
  for (;;) // A Task shall never return or exit.
    {   
        bool stateOfWiFiScan = wifiScanSync;
        
         if (btScanAsync) {
                 wifiScanSync = false; 
                 wifi_sniffer_pause(wifiScanSync); // turn off wif scanning while bt scann is happening
                 vTaskDelay(2000/ portTICK_PERIOD_MS);
                 btRunScan();
                 wifiScanSync = stateOfWiFiScan; // restore the sate of the wifi scan function
                 } // finish run bluetooth async scan
        
         vTaskDelay(10000 / portTICK_PERIOD_MS); //wait 10 seconds
         wifi_sniffer_pause(wifiScanSync);  // turn wifi scanning on or off depending on the flag
         packetCount = 0;     
         vTaskDelay(5000 / portTICK_PERIOD_MS); //wait 5 seconds            
         databaseloopHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); //find the high watermark for the datalogger task
        
         
       
  }// infinite loop
} 
