#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <Time.h>

using namespace Time;

RTC_DS1307 rtc;

void Time::init_rtc_module()
{

    if (rtc.begin())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#ifdef DEBUG
        Serial.println("[Info] I2C for DS3231SN");
        Serial.println("[Info] Finished RTC Setup");
#endif
    }
#ifdef DEBUG
    else
    {
        Serial.println("[Error Failed RTC Setup!");
    }
#endif
}

String Time::get_time_string(String format)
{
    DateTime now = rtc.now();
    char arr[format.length() + 1];
    strcpy(arr, format.c_str());
    String timestring = now.toString(arr);

#ifdef DEBUG
    Serial.print("[Info] Timestring: ");
    Serial.println(timestring);
#endif

    return timestring;
}