// https://github.com/adafruit/RTClib/blob/master/src/RTClib.h

#include <Wire.h>
#include "RTClib.h"
#include "time.h"

//extern RTC_PCF8563 rtc;
RTC_PCF8563 rtc;

char week_day[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

bool time_begin(void)
{
  bool ret = false;
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //while (1) delay(10);
  } 
  else 
  {
    ret = true;
    time_lost_power(true);
    time_print();
  }
  return ret;
}

void time_lost_power(bool force_new_time)
{
  if (rtc.lostPower() || force_new_time) 
  {
      Serial.println("RTC is NOT initialized, let's set the time!");
      // When time needs to be set on a new device, or after a power loss, the
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      //rtc.adjust(DateTime(2023, 12, 13, 3, 0, 0));
      //
      // Note: allow 2 seconds after inserting battery or applying external power
      // without battery before calling adjust(). This gives the PCF8523's
      // crystal oscillator time to stabilize. If you call adjust() very quickly
      // after the RTC is powered, lostPower() may still return true.


    // When the RTC was stopped and stays connected to the battery, it has
    // to be restarted by clearing the STOP bit. Let's do this to ensure
    // the RTC is running.
    rtc.start();
  }

}

void time_to_string(String *Str)
{
   DateTime now = rtc.now();

  *Str = String("");
  *Str += now.year();
  *Str += String("-");
  *Str += now.month();
  *Str += String("-");
  *Str += now.day();
  *Str += String(" ");
  *Str += now.hour();
  *Str += String(":");
  *Str += now.minute();
  *Str += String(":");
  *Str += now.second();
  *Str += String(":");
}

uint32_t time_get_epoc_time(void)
{
    DateTime now = rtc.now();
    return now.unixtime();
}

void time_print(void)
{
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(week_day[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
}