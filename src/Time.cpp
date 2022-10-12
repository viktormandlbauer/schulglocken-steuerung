#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

#include <Time.h>

RTC_DS1307 rtc;

void init_rtc_module()
{
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }
    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

char *get_time_string(char format[])
{
    DateTime now = rtc.now();
    char *timestring = now.toString(format);
    return timestring;
}