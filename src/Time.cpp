#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include "RTClib.h"
#include <Wire.h>
#include <TimeLib.h>
#include <SPI.h>
#include "Time.h"

#include <LinkedList.h>

using namespace Time;

RTC_DS3231 rtc;

struct alert
{
    uint16_t time_trigger;
    bool pause;
};

LinkedList<alert> scheduled;

uint16_t Time::get_passing_seconds(byte hour, byte minute, byte second)
{
    return hour * 3600 + second;
}

void Time::add_alert(byte hour, byte minute, byte second)
{

    alert new_alert;
    new_alert.time_trigger = get_passing_seconds(hour, minute, second);
    new_alert.pause = false;
    scheduled.add(new_alert);
}

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
