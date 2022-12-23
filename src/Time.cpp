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

// Alarme
uint16_t alarms[100];
uint8_t alarm_type[100];

// Ausnahmen
uint16_t exception_date_begin[20];
uint16_t exception_date_end[20];

time_t time_provider()
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
