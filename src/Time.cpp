#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

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

String get_time_string(String timeformat)
{
    DateTime now = rtc.now();
    char arr[timeformat.length() + 1];
    strcpy(arr, timeformat.c_str());

    String timestring = now.toString(arr);

#ifdef DEBUG
    Serial.print("[Info] Timestring: ");
    Serial.println(timestring);
#endif 

    return timestring;
}