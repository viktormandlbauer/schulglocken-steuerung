#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include "RTClib.h"
#include <Wire.h>
#include <TimeLib.h>
#include <SPI.h>
#include "Time.h"

using namespace Time;

RTC_DS3231 rtc;
uint16_t alarms[100];

time_t Time::time_provider()
{
    return rtc.now().unixtime();
}

bool Time::init_rtc_module()
{
    Wire.begin();
    while (!rtc.begin())
        ;

    // Konfiguration der Synchronisation mit dem RTC Modul
    setSyncProvider(time_provider);
    setSyncInterval(5);

    if (timeStatus() != timeSet)
    {
#ifdef DEBUG
        Serial.println("[Error] Time not synced!");
#endif
        return false;
    }
    else
    {
#ifdef DEBUG
        Serial.println("[Info] Time synced!");
#endif
        return true;
    }
}
