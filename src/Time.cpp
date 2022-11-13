#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <Time.h>

using namespace Time;

RTC_DS3231 rtc;

bool Time::init_rtc_module()

{

    if (rtc.begin())
    {
#ifdef DEBUG
        Serial.println("[Info] Finished RTC Setup");
#endif
    }
    else
    {

        // TODO - RTC Modul nicht funktionsfähig
#ifdef DEBUG
        Serial.println("[Error Failed RTC Setup!");
#endif
    }

    if (rtc.lostPower())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#ifdef DEBUG
        Serial.println("[Info] Adjusted time after power loss");
#endif
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

int *Time::get_time_values(int arr[])
{
    DateTime now = rtc.now();
    arr[0] = now.hour();
    arr[1] = now.minute();
    arr[2] = now.second();
    return arr;
}
