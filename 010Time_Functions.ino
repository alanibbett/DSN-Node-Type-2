

void setupTime(){
  // get the Date and Time from the RTC
    RTC_TimeTypeDef TimeStruct;
    RTC_DateTypeDef DateStruct;
    M5.Rtc.GetTime(&TimeStruct);    //Get the time of the real-time clock.
    M5.Rtc.GetDate(&DateStruct);    //Get the date of the real-time clock.
    setTime(TimeStruct.Hours, TimeStruct.Minutes, TimeStruct.Seconds, DateStruct.Date, DateStruct.Month, DateStruct.Year);
    setSyncProvider(timeSyncRTC);
}


// This function takes a human readable time and date date value
// and returns a UNIX epoch value
time_t timeSyncRTC()
{
  RTC_TimeTypeDef TimeStruct;
  RTC_DateTypeDef DateStruct;
  M5.Rtc.GetTime(&TimeStruct);
  M5.Rtc.GetDate(&DateStruct);    //Get the date of the real-time clock.
  tmElements_t tmSet; // create a structure in the right format for the conversion function
  tmSet.Year = DateStruct.Year - 1970;
  tmSet.Month = DateStruct.Month;
  tmSet.Day = DateStruct.Date;
  tmSet.Hour = TimeStruct.Hours;
  tmSet.Minute = TimeStruct.Minutes;
  tmSet.Second = TimeStruct.Seconds;
  return makeTime(tmSet); 
}
