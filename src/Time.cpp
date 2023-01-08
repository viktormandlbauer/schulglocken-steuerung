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
uint16_t alarms[64];
uint8_t alarm_type[64];
uint64_t triggered;

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
    /**
     * Initiierung des RTC Moduls
    */
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

void get_timestring(int hour, int minute, int second, char time_string[9])
{
    /**
     * Funktion zur Formatierung der Stunde, Minute & Sekunde in das "HH:MM:SS" Format
     */

    // Konviertung der Stunde
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

    // Konvertierung der Minute
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

    // Konvertierung der Sekunden
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

void convert_alarm_to_timestring(uint16_t alarm, char time_string[9])
{
    // Schreibt einen Alarm in ein character array.
    get_timestring(alarm / 60, alarm % 60, 0, time_string);
}

void Time::get_current_timestring(char time_string[9])
{
    // Schreibt die in "HH:MM:SS" formatierte Zeit in ein character array der länge 9
    get_timestring(hour(), minute(), second(), time_string);
}

uint8_t Time::add_alarm(uint8_t hour, uint8_t minute, uint8_t alarm_type)
{
    alarms[alarm_count] = convert_time_to_alarm(hour, minute);
    alarm_count += 1;
    return alarm_count;
}

uint8_t Time::remove_alarm_at_index(uint8_t index)
{
    // Entfernt einen Alarm und den Typ am bestimmten Index
    for (int i = index; i < alarm_count - 1; i++)
    {
        alarms[i] = alarms[i + 1];
        alarm_type[i] = alarm_type[i + 1];
    }
    alarm_count -= 1;
    return alarm_count;
}

uint8_t Time::get_alarms_strings(char time_string[][9])
{
    // Füllt ein zweidimensionales character array mit den formatierten Alarmzeiten und gibt die Anzahl der Alarme zurück.
    for (int i = 0; i < alarm_count; i++)
    {
        convert_alarm_to_timestring(alarms[i], time_string[i]);
    }
    return alarm_count;
}

uint8_t Time::get_alarm_count()
{
    // Gibt die Anzahl der aktuell eingestellten Alarme zurück
    return alarm_count;
}

uint16_t Time::get_minutes_passed()
{
    // Gibt die Minuten nach 00:00 als Ganzzahl zurück
    return convert_time_to_alarm(hour(), minute());
}

bool active_alarm = false;
time_t alarm_start_time = 0;
time_t alarm_end_time = 0;

time_t get_alarm_end_time()
{
    //  5 Sekunden Alarm für Tests
    // TODO: Verschiedene Arten von Alarmen (Rhytmus usw.)
    return alarm_start_time + 3;
}

void ring()
{
    if (active_alarm == false)
    {
        /**
         * Wenn kein aktiver Alarm besteht, 
         * wird die Endzeit des Alarms berechnet und das Läuten initiert.
        */
        alarm_start_time = time_provider();
        alarm_end_time = get_alarm_end_time();
        active_alarm = true;
        analogWrite(A2, 1023);
    }
    if (alarm_end_time < time_provider())
    {
        /**
         * Das Läuten endet sobald die Endzeit des Alarms überschritten worden ist.
        */
        active_alarm = false;
        analogWrite(A2, LOW);
    }
}

// Funktionen zur Überprüfung des Status eines Alarms
bool is_triggered(uint8_t index) { return triggered & (0b1 << index); }
void set_triggered(uint8_t index) { triggered |= 0b1 << index; }

// Allgemeine Überprüfung eines Alarms
bool Time::check_alarm()
{
    // Testet ob Alarm aktiv ist
    if (active_alarm)
    {
#ifdef DEBUG
        Serial.println("[Info] Alarm is active!");
#endif
        ring();
        return true;
    }
    else
    {
        // Holt sich die aktuelle Zeit in Minuten nach 00:00
        uint16_t current_time = get_minutes_passed();
#ifdef DEBUG
        Serial.print("[Info] Current time: ");
        Serial.println(current_time);
#endif
        // Vergleicht die aktuellen Zeiten mit den Alarmzeiten
        for (uint8_t i = 0; i < alarm_count; i++)
        {
#ifdef DEBUG
            Serial.print("[Info] Active alarm: ");
            Serial.println(alarms[i]);
#endif
            if (!is_triggered(i))
            {
                if (current_time == alarms[i])
                {
                    // Flag um ein erneutes Läuten in der selben Minute zu verhindern
                    set_triggered(i);
#ifdef DEBUG
                    Serial.print("[Info] Alarm triggered: ");
                    Serial.println(alarms[i]);
#endif
                    ring();
                    return true;
                }
            }
        }
        return false;
    }
}
