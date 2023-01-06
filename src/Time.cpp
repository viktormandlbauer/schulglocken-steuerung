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
uint8_t alarm_count;
uint16_t alarms[100];
uint8_t alarm_type[100];

// Ausnahmen
uint8_t exception_count;
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

    // Set system time as rtc time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

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

void get_timestring(int hour, int minute, int second, char *time_string)
{
    if (hour < 10)
    {
        time_string[0] = '0';
        time_string[1] = hour + '0';
    }
    else
    {
        time_string[0] = (char)hour / 10 + '0';
        time_string[1] = hour % 10 + '0';
    }

    time_string[2] = ':';

    if (minute < 10)
    {
        time_string[3] = '0';
        time_string[4] = minute + '0';
    }
    else
    {
        time_string[3] = (char)minute / 10 + '0';
        time_string[4] = minute % 10 + '0';
    }

    time_string[5] = ':';

    if (second < 10)
    {
        time_string[6] = '0';
        time_string[7] = second + '0';
    }
    else
    {
        time_string[6] = (char)second / 10 + '0';
        time_string[7] = second % 10 + '0';
    }
}

uint16_t convert_time_to_alarm(uint8_t hour, uint8_t minute)
{
    // Interne Funtkion für die Konvertierung von Stunden und Minuten in vergehende Minuten von 00:00:00 weg
    return hour * 60 + minute;
}

void convert_alarm_to_timestring(uint16_t alarm, char *time_string)
{
    get_timestring(alarm / 60, alarm % 60, 0, time_string);
}

uint16_t Time::get_minutes_passed()
{
    return convert_time_to_alarm(hour(), minute());
}

void Time::get_current_timestring(char time_string[9])
{
    // Schreibt die Zeit in "HH:MM:SS" Format in ein Char Array
    // char time_string[9];
    get_timestring(hour(), minute(), second(), time_string);
}

uint8_t Time::add_alarm(uint8_t hour, uint8_t minute, uint8_t alarm_type)
{
    alarms[alarm_count] = convert_time_to_alarm(hour, minute);
    alarm_count += 1;
    return alarm_count;
}

uint8_t Time::remove_alarm(uint8_t hour, uint8_t minute)
{
    uint16_t time_to_remove = convert_time_to_alarm(hour, minute);
    uint8_t index_to_remove = alarm_count;

    int i;
    for (i = 0; i < alarm_count; i++)
    {
        if (alarms[i] == time_to_remove)
        {
            index_to_remove = i;
            break;
        }
    }
    if (index_to_remove != alarm_count)
    {
        for (i = index_to_remove; i < alarm_count - 1; i++)
        {
            alarms[i] = alarms[i + 1];
            alarm_type[i] = alarm_type[i + 1];
        }
        alarm_count -= 1;
        return alarm_count;
    }
    return alarm_count;
}

void Time::get_alarms_strings(char time_string[][9])
{

    for (int i = 0; i < alarm_count; i++)
    {
        convert_alarm_to_timestring(alarms[i], time_string[i]);
    }
}

uint8_t Time::get_alarm_count()
{
    return alarm_count;
}
