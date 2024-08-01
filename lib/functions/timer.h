#include <Timezone.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimerMs.h>


#define TIMER_COUNT 5

struct Timer
{
  bool enable;
  byte action;
  byte hours;
  byte minutes;
  byte seconds;
};


struct Time{
  byte timezone = 1;
  uint32 bootTime;
  Timer timer[TIMER_COUNT];
};


  Time time;

  // Определение NTP-клиента для получения времени
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);


    TimerMs handleTimerDelay;

int convertTimezoneToOffset(){
  if(time.timezone == 1 ) return -43200; //-12:00 
  if(time.timezone == 2 ) return -39600; //-11:00
  if(time.timezone == 3 ) return -36000; //-10:00
  if(time.timezone == 4 ) return -34200; //-09:30
  if(time.timezone == 5 ) return -32400; //-09:00
  if(time.timezone == 6 ) return -28800; //-08:00
  if(time.timezone == 7 ) return -25200; //-07:00
  if(time.timezone == 8 ) return -21600; //-06:00
  if(time.timezone == 9 ) return -18000; //-05:00
  if(time.timezone == 10 ) return -14400; //-04:00
  if(time.timezone == 11 ) return -12600; //-03:30
  if(time.timezone == 12 ) return -10800; //-03:00
  if(time.timezone == 13 ) return -7200; //-02:00
  if(time.timezone == 14 ) return -3600; //-01:00
  if(time.timezone == 15 ) return 0;     //UTC
  if(time.timezone == 16 ) return 3600;  //+01:00
  if(time.timezone == 17 ) return 7200;  //+02:00
  if(time.timezone == 18 ) return 10800; //+03:00
  if(time.timezone == 19 ) return 12600; //+03:30
  if(time.timezone == 19 ) return 14400; //+04:00
  if(time.timezone == 20 ) return 16200; //+04:30
  if(time.timezone == 21 ) return 18000; //+05:00
  if(time.timezone == 22 ) return 19800; //+05:30
  if(time.timezone == 23 ) return 20700; //+05:45
  if(time.timezone == 24 ) return 21600; //+06:00
  if(time.timezone == 25 ) return 23400; //+06:30
  if(time.timezone == 26 ) return 25200; //+07:00
  if(time.timezone == 27 ) return 28800; //+08:00
  if(time.timezone == 28 ) return 31500; //+08:45
  if(time.timezone == 29 ) return 32400; //+09:00
  if(time.timezone == 30 ) return 34200; //+09:30
  if(time.timezone == 31 ) return 36000; //+10:00
  if(time.timezone == 32 ) return 37800; //+10:30
  if(time.timezone == 33 ) return 39600; //+11:00
  if(time.timezone == 34 ) return 43200; //+12:00
  if(time.timezone == 35 ) return 46800; //+13:00
  if(time.timezone == 36 ) return 50400; //+14:00

  return 0;
}


void timerHandle(){
  int hours   = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  for(int i=0; i<TIMER_COUNT; i++){
    if( time.timer[i].enable  == true &&
        time.timer[i].hours   == hours &&
        time.timer[i].minutes == minutes &&
        time.timer[i].seconds == seconds)
      {  

        Serial.println("Timer "+String(i)+" activating");
        /*if(time.timer[i].action == 0){Relay1.SetState(true);}
        if(time.timer[i].action == 1){Relay1.SetState(false);}
        if(time.timer[i].action == 2){Relay1.ResetState();}*/
      }
  }
}

void timer_loop(){
    timeClient.update();
    handleTimerDelay.tick();
}

void timerSetup(){
      //NTP 
  Serial.println("Starting NTP");
  timeClient.setPoolServerName("pool.ntp.org");
  timeClient.setTimeOffset(convertTimezoneToOffset());
  timeClient.begin();

  // Timers handler
  Serial.println("Starting timers handler");
  handleTimerDelay.setTime(1000);
  handleTimerDelay.attach(timerHandle);
  handleTimerDelay.start();

}