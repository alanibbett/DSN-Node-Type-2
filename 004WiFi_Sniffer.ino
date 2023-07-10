/*
 * 
 * 
 d8888b. d88888b  .o88b. db       .d8b.  d8888b.  .d8b.  d888888b d888888b  .d88b.  d8b   db .d8888. 
88  `8D 88'     d8P  Y8 88      d8' `8b 88  `8D d8' `8b `~~88~~'   `88'   .8P  Y8. 888o  88 88'  YP 
88   88 88ooooo 8P      88      88ooo88 88oobY' 88ooo88    88       88    88    88 88V8o 88 `8bo.   
88   88 88~~~~~ 8b      88      88~~~88 88`8b   88~~~88    88       88    88    88 88 V8o88   `Y8b. 
88  .8D 88.     Y8b  d8 88booo. 88   88 88 `88. 88   88    88      .88.   `8b  d8' 88  V888 db   8D 
Y8888D' Y88888P  `Y88P' Y88888P YP   YP 88   YD YP   YP    YP    Y888888P  `Y88P'  VP   V8P `8888Y'
 * 
 * 
 * 
 * 
 */

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

char *subTypes[] = {
    "associationRequest",
    "associationResponse",
    "reassociationRequest",
    "reassociationResponse",
    "probeRequest",
    "probeResponse",
    "timingAdvertisement",
    "reserved",
    "beacon",
    "udef",
    "disassociation",
    "authentication",
    "deauthentication",
    "udef",
    "action"
    "udef"
    };


/*
 * 
 * 
 db    db d888888b d888888b db      d888888b d888888b d888888b d88888b .d8888. 
88    88 `~~88~~'   `88'   88        `88'   `~~88~~'   `88'   88'     88'  YP 
88    88    88       88    88         88       88       88    88ooooo `8bo.   
88    88    88       88    88         88       88       88    88~~~~~   `Y8b. 
88b  d88    88      .88.   88booo.   .88.      88      .88.   88.     db   8D 
~Y8888P'    YP    Y888888P Y88888P Y888888P    YP    Y888888P Y88888P `8888Y'
 * 
 * 
 */

//*******************************************************
// Find the centre frequency of a wifi channel
// returns an int
uint16_t centreFrequency(uint8_t channel){
  if (channel < 14){
     return (2407 + (5*channel));
    }
  else if (channel == 14) {
     return 2484; 
   }
  else {
    return 0;
  }
  
}// centreFrequency


/*******************************************************
d88888b d8888b. d8888b.  .d88b.  d8888b.      db   db  .d8b.  d8b   db d8888b. db      d88888b d8888b. 
88'     88  `8D 88  `8D .8P  Y8. 88  `8D      88   88 d8' `8b 888o  88 88  `8D 88      88'     88  `8D 
88ooooo 88oobY' 88oobY' 88    88 88oobY'      88ooo88 88ooo88 88V8o 88 88   88 88      88ooooo 88oobY' 
88~~~~~ 88`8b   88`8b   88    88 88`8b        88~~~88 88~~~88 88 V8o88 88   88 88      88~~~~~ 88`8b   
88.     88 `88. 88 `88. `8b  d8' 88 `88.      88   88 88   88 88  V888 88  .8D 88booo. 88.     88 `88. 
Y88888P 88   YD 88   YD  `Y88P'  88   YD      YP   YP YP   YP VP   V8P Y8888D' Y88888P Y88888P 88   Y
// generic error handler for the ESP wifi functinos in the SDK
// returns an error object
*/
esp_err_t event_handler(void *ctx, system_event_t *event)
{
  return ESP_OK;
}

/*******************************************************
.d8888. d8b   db d888888b d88888b d88888b d88888b d8888b.      .d8888. d88888b d888888b db    db d8888b. 
88'  YP 888o  88   `88'   88'     88'     88'     88  `8D      88'  YP 88'     `~~88~~' 88    88 88  `8D 
`8bo.   88V8o 88    88    88ooo   88ooo   88ooooo 88oobY'      `8bo.   88ooooo    88    88    88 88oodD' 
  `Y8b. 88 V8o88    88    88~~~   88~~~   88~~~~~ 88`8b          `Y8b. 88~~~~~    88    88    88 88~~~   
db   8D 88  V888   .88.   88      88      88.     88 `88.      db   8D 88.        88    88b  d88 88      
`8888Y' VP   V8P Y888888P YP      YP      Y88888P 88   YD      `8888Y' Y88888P    YP    ~Y8888P' 88   
// 
// Initialise the wifi sniffer
*/

void wifi_sniffer_init(void)
{
 
  tcpip_adapter_init();
  ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_country(&wifi_country) ); /* set country for channel range [1, 13] */
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
  ESP_ERROR_CHECK( esp_wifi_start() );
  esp_wifi_set_promiscuous(wifiScanSync);
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

//*******************************************************
// Pause the wifi sniffer
// 
void wifi_sniffer_pause(bool mode)
{
  
  esp_wifi_set_promiscuous(mode);
  
}



//*******************************************************
// This function is an important debugging function and
// is used to print out an entire packet as required.
// Prints to the serial port in the hex format used by
// the shark

void process_promisc(const void* buf, uint16_t len)
{   
  uint8_t HEX_COLSIZE = 16; // change this to change the width of the printout
  // 16 matches wire shark
    char* buflen = (char*) buf + len;
    uint8_t hexdump_cols = 0;
    uint8_t offset = 0;

    // RAW packet
    for (char* ptr = (char*)buf; ptr < buflen; ptr++) printf("%c", *ptr);
    printf("\n\n");
    
    // Hexdump (wireshark-friendly)
    for (char* ptr = (char*) buf; ptr < buflen; ptr+=HEX_COLSIZE) {
        // print offset
        printf(" %06X ", offset);

        for (hexdump_cols=0; hexdump_cols < HEX_COLSIZE; hexdump_cols++) 
            printf(" %02X", *(ptr+hexdump_cols*sizeof(char)));

        offset = offset + HEX_COLSIZE;
        printf("\n");
    }

    printf("\n\r");
    //return NULL;
} // Process promisc packet


//*******************************************************
// This function returns the offset in an 80211 management frame where the 
// ssid is located.
uint8_t locateSSIDSize(uint8_t frameSubType){
      if ((frameSubType == 5) || (frameSubType == 8)){
        return 37;
      }
      if ((frameSubType == 4)){
        return 25;
      }
      return 0;
  
}// locate SSID size


//*******************************************************
// extract the ssid from the raw packet. If the SSID length is 0
// return hidden
void extractSSID(char* ssid, char* packet, uint8_t ssidlen){

          if (ssidlen > 0) {
            memcpy(ssid,packet,ssidlen);
            ssid[ssidlen] = '\0';
          }
  
}// extract ssid

//*******************************************************
// a utility function to print a block of memory to the
//Serial console. No formattting is applied
void printDataSpan(uint16_t start, uint16_t size, uint8_t* data) {
  for(uint16_t i = start; i < start+size; i++) {
    //Serial.print(data[i]);
    Serial.write(data[i]);
  }
}

/*
 * 
 .d8888. d8b   db d888888b d88888b d88888b d88888b d8888b. 
88'  YP 888o  88   `88'   88'     88'     88'     88  `8D 
`8bo.   88V8o 88    88    88ooo   88ooo   88ooooo 88oobY' 
  `Y8b. 88 V8o88    88    88~~~   88~~~   88~~~~~ 88`8b   
db   8D 88  V888   .88.   88      88      88.     88 `88. 
`8888Y' VP   V8P Y888888P YP      YP      Y88888P 88   YD
 * 
 * this is the call back function for the wifi sniffer the ESP32 wifi library
 * calls this function when it has a packet. This functions should execute quickly 
 * and leave no trace...
 * 
 */
//*******************************************************
void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
  if (type != WIFI_PKT_MGMT)
    return;
  // split the raw packet up into ots bits
  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;
  
  // work out the type of frame it is
  unsigned int frameControl = (unsigned int)hdr->frame_ctrl;

  uint8_t version      = (frameControl & 0b0000000000000011) >> 0;
  uint8_t frameType    = (frameControl & 0b0000000000001100) >> 2;
  uint8_t frameSubType = (frameControl & 0b0000000011110000) >> 4;
  uint8_t toDS         = (frameControl & 0b0000000100000000) >> 8;
  uint8_t fromDS       = (frameControl & 0b0000001000000000) >> 9;
  char ssid[33]; // max length of an ssid plus the /0
  char sql[255]; // string to hold the database query  
  BaseType_t qStatus;


if ((frameSubType==4) || (frameSubType==5)) {   // check for probe or probe response     
      uint8_t   ssidOffset = locateSSIDSize(frameSubType) + 1; // actual ssid starts after the size field - if subtype does not have an ssid this is invalid
      uint8_t*  ssidlengthlocation = (uint8_t*)ipkt + locateSSIDSize(frameSubType); // find the location of the length field
      uint8_t   ssidlength = *ssidlengthlocation; // extract the ssid length from the frame
      char*     ssidBegin = (char*) ipkt + ssidOffset;// this is a pointer to the begining of the ssid value
      char*     buflen = ssidBegin + ssidlength;// this is a pointer to the end of the ssid value
      
      extractSSID(ssid, ssidBegin,ssidlength); // pull the ssid out of the frame
      snprintf(sql,255,"insert into probes (bssid,essid,date) values('%02x:%02x:%02x:%02x:%02x:%02x','%s','%d');",hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],hdr->addr3[3],hdr->addr3[4],hdr->addr3[5],ssid,now());;
      logProbeData(sql);
      packetCount++;

      
     } // log probe sub type 4 or 5
 
if ((frameSubType==8)) {   // check for beacons     
      uint8_t   ssidOffset = locateSSIDSize(frameSubType) + 1; // actual ssid starts after the size field - if subtype does not have an ssid this is invalid
      uint8_t*  ssidlengthlocation = (uint8_t*)ipkt + locateSSIDSize(frameSubType); // find the location of the length field
      uint8_t   ssidlength = *ssidlengthlocation; // extract the ssid length from the frame
      char*     ssidBegin = (char*) ipkt + ssidOffset;// this is a pointer to the begining of the ssid value
      char*     buflen = ssidBegin + ssidlength;// this is a pointer to the end of the ssid value  
      extractSSID(ssid, ssidBegin,ssidlength); // pull the ssid out of the frame
      snprintf(sql,255,"insert into wifis (bssid,essid,encryption,signal,longitude,latitude,frequency,channel,mode,date,gps) values('%02x:%02x:%02x:%02x:%02x:%02x','%s','%d','%d','%-.6f','%-.6f','%d','%d','%d','%d','%d');",
                       hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],hdr->addr3[3],hdr->addr3[4],hdr->addr3[5],
                       ssid,1,ppkt->rx_ctrl.rssi,longitude,latitude,centreFrequency(ppkt->rx_ctrl.channel),ppkt->rx_ctrl.channel,1,now(),gpsLockedOK());
      logProbeData(sql);
      //Serial.println(sql);
      packetCount++;

     } // log probe sub type 8


// switch off wifi monitoring after predetermined number of bn packets
  if (packetCount > maxWifiPackets -1 ) {
    Serial.printf ("Maximum packet count reached %d packets captured. Switiching off sniffer\n\r",packetCount);
    Serial.println("Use menu to switch back on");
    wifiScanSync = false;
    wifi_sniffer_pause(false);
    packetCount = 0;
  }



if (printWiFiScan) // iff we want to print to the consol then do this.....
   {
      //process_promisc(ipkt,ppkt->rx_ctrl.sig_len); // print raw packets for debugging
  
    if ((frameSubType==4) || (frameSubType==5) || (frameSubType==8)) {   // check for beacon, probe or probe response
      uint8_t   ssidOffset = locateSSIDSize(frameSubType) + 1; // actual ssid starts after the size field
      uint8_t*  ssidlengthlocation = (uint8_t*)ipkt + locateSSIDSize(frameSubType); // find the location of the length field
      uint8_t   ssidlength = *ssidlengthlocation; // extract the ssid length from the frame
      char ssid[33]; // max length of an ssid plus the /0
      char* ssidBegin = (char*) ipkt + ssidOffset;// this is a pointer to the begining of the ssid value
      char* buflen = ssidBegin + ssidlength;// this is a pointer to the end of the ssid value

      extractSSID(ssid, ssidBegin,ssidlength); // pull the ssid out of the frame
    
      printf("%s, CHAN=%02d, RSSI=%02d, Frequency %i MHz,"
      " BSSID=%02x:%02x:%02x:%02x:%02x:%02x,"
      " ESSID=%s \r\n",
      subTypes[frameSubType],
      ppkt->rx_ctrl.channel,
      ppkt->rx_ctrl.rssi,
      centreFrequency(ppkt->rx_ctrl.channel),
      // ADDR3 
      hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
      hdr->addr3[3],hdr->addr3[4],hdr->addr3[5],
      ssid ); 
   
      
     } // if sub type
   }// if print wifiscan
 
} // packet handler




//*******************************************************
// Set the channel for the sniffer
//
void wifi_sniffer_set_channel(uint8_t channel){
   
          esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

//*******************************************************
// Extracts the type of frame as an english word
//
const char * wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
  switch(type) {
  case WIFI_PKT_MGMT: return "MGMT";
  case WIFI_PKT_DATA: return "DATA";
  default:  
  case WIFI_PKT_MISC: return "MISC";
  }
}



void changeWifiChannel(){
          wifi_sniffer_set_channel(channel); // change the channel every now and then
          channel = (channel % WIFI_CHANNEL_MAX) + 1; 

  
}
/*
 * 
 * 
 .d8888. d8b   db d888888b d88888b d88888b d88888b d8888b.      d888888b  .d8b.  .d8888. db   dD 
88'  YP 888o  88   `88'   88'     88'     88'     88  `8D      `~~88~~' d8' `8b 88'  YP 88 ,8P' 
`8bo.   88V8o 88    88    88ooo   88ooo   88ooooo 88oobY'         88    88ooo88 `8bo.   88,8P   
  `Y8b. 88 V8o88    88    88~~~   88~~~   88~~~~~ 88`8b           88    88~~~88   `Y8b. 88`8b   
db   8D 88  V888   .88.   88      88      88.     88 `88.         88    88   88 db   8D 88 `88. 
`8888Y' VP   V8P Y888888P YP      YP      Y88888P 88   YD         YP    YP   YP `8888Y' YP   YD
 * 
 * this is the actual wifi sniffer FREERTOS task. This is launched at the start of the application
 * and never returns
 * 
 *//*

void TaskWiFiSniff(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  Serial.println("Wifi Sniffer task Started.");
  for (;;) // A Task shall never return or exit.
    {   
          vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS); // relinquish control for other tasks to play
          wifi_sniffer_set_channel(channel); // change the channel every now and then
          channel = (channel % WIFI_CHANNEL_MAX) + 1; 
          wifiHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); //find the high watermark for the guitask
  }
}
*/
