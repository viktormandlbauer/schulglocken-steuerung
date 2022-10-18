#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <Time.h>

using namespace Time;

RTC_DS1307 rtc;

void Time::init_rtc_module()
{
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
    }
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