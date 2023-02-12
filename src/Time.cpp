// Einstellung für den DEBUG Mode
#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

// Einbindung externer Libraries
#include "RTClib.h"
#include <Wire.h>
#include <TimeLib.h>
#include <SPI.h>

// Angabe des Header Files
#include "Time.h"

// Verwendung des Namespaces
using namespace Time;

// RTC Objekt aus der RTClib
RTC_DS3231 rtc;

// Array mit Alarmtypen
uint32_t alarm_types[MAXIMUM_AMOUNT_ALARM_TYPES];

// 64 bit Wert für die Abhandlung der Alarme
static uint64_t triggered;

// Anzahl der Ausnahmen
uint8_t exception_count;

// Array mit 16 Bit Werten für Ausnahme Beginn-Tag
uint16_t exception_date_begin[MAXIMUM_AMOUNT_DAY_EXCEPTIONS];

// Aray mit 16 Bit Werten für Ausnahme End-Tag
uint16_t exception_date_end[MAXIMUM_AMOUNT_DAY_EXCEPTIONS];

time_t time_provider()
{
    // Rückgabe der aktuellen Unix Zeit
    // Vergangene Sekunden nach dem 1.Januar 1970, 00:00 Uhr UTC
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

    // Set flash time as system time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Konfiguration der Synchronisation mit dem RTC Modul
    setSyncProvider(time_provider);
    setSyncInterval(5);

    if (timeStatus() != timeSet)
    {
#ifdef DEBUG
        Serial.println("[Error] (Time) Time not synced!");
#endif
        return false;
    }
    else
    {
#ifdef DEBUG
        Serial.println("[Info] (Time) Time synced!");
#endif
        return true;
    }
}

void get_timestring(int hour, int minute, int second, char *buf)
{
    /**
     * Funktion zur Formatierung der Stunde, Minute & Sekunde in das "HH:MM:SS" Format
     */

    // Konviertung der Stunde
    if (hour < 10)
    {
        buf[0] = '0';
        buf[1] = hour + '0';
    }
    else
    {
        buf[0] = (char)hour / 10 + '0';
        buf[1] = hour % 10 + '0';
    }

    // Konvertierung der Minute
    buf[2] = ':';
    if (minute < 10)
    {
        buf[3] = '0';
        buf[4] = minute + '0';
    }
    else
    {
        buf[3] = (char)minute / 10 + '0';
        buf[4] = minute % 10 + '0';
    }

    // Konvertierung der Sekunden
    buf[5] = ':';
    if (second < 10)
    {
        buf[6] = '0';
        buf[7] = second + '0';
    }
    else
    {
        buf[6] = (char)second / 10 + '0';
        buf[7] = second % 10 + '0';
    }
    buf[8] = '\0';
}
void get_timestring(int hour, int minute, char *buf)
{
    /**
     * Funktion zur Formatierung der Stunde, Minute & Sekunde in das "HH:MM" Format
     */

    // Konviertung der Stunde
    if (hour < 10)
    {
        buf[0] = '0';
        buf[1] = hour + '0';
    }
    else
    {
        buf[0] = (char)hour / 10 + '0';
        buf[1] = hour % 10 + '0';
    }

    // Konvertierung der Minute
    buf[2] = ':';
    if (minute < 10)
    {
        buf[3] = '0';
        buf[4] = minute + '0';
    }
    else
    {
        buf[3] = (char)minute / 10 + '0';
        buf[4] = minute % 10 + '0';
    }
    buf[5] = '\0';
}

uint16_t convert_time_to_alarm(uint8_t hour, uint8_t minute)
{
    // Interne Funtkion für die Konvertierung von Stunden und Minuten in vergehende Minuten von 00:00:00 weg
    return hour * 60ul + minute;
}

void Time::get_current_timestring(char output[9])
{
    // Schreibt die in "HH:MM:SS" formatierte Zeit in ein character array der länge 9
    get_timestring(hour(), minute(), second(), output);
}

void Time::get_alarm_string(uint16_t alarm, char output[6])
{
    get_timestring(alarm / 60, alarm % 60, output);
}

uint8_t Time::add_alarm(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t alarm_count, uint8_t hour, uint8_t minute, uint8_t alarm_type)
{
    alarms[alarm_count] = convert_time_to_alarm(hour, minute);
    alarms_type_assignment[alarm_count] = alarm_type;
#ifdef DEBUG
    Serial.print("[Info] (Time) Added alarm: ");
    Serial.println(alarms[alarm_count]);

    Serial.print("[Info] (Time) with alarm type: ");
    Serial.println(alarms_type_assignment[alarm_count]);

#endif
    return alarm_count += 1;
}

uint8_t Time::remove_alarm_at_index(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t alarm_count, uint8_t index)
{
    // Entfernt einen Alarm und den Typ am bestimmten Index
    for (int i = index; i < alarm_count - 1; i++)
    {
        alarms[i] = alarms[i + 1];
        alarms_type_assignment[i] = alarms_type_assignment[i + 1];
    }
    alarm_count -= 1;
    return alarm_count;
}

void Time::get_alarms_strings(uint16_t alarms[], uint8_t alarm_count, char output[][6])
{

    // Füllt ein zweidimensionales character array mit den formatierten Alarmzeiten und gibt die Anzahl der Alarme zurück.
    for (int i = 0; i < alarm_count; i++)
    {
        get_timestring(alarms[i] / 60, alarms[i] % 60, output[i]);
    }
}

/*
Setter und Getter für Alarme
*/

uint16_t Time::get_minutes_passed()
{
    // Gibt die Minuten nach 00:00 als Ganzzahl zurück
    return convert_time_to_alarm(hour(), minute());
}

// Funktionen zur Überprüfung des Status eines Alarms
bool is_triggered(uint8_t index) { return triggered & (0b1 << index); }
void set_triggered(uint8_t index) { triggered |= 0b1 << index; }

uint8_t current_alarm_type = 0;
uint32_t position = 0;
bool finished = true;

// Allgemeine Überprüfung eines Alarms
bool Time::check_alarm(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t alarm_count)
{
    // Testet ob Alarm aktiv ist
    if (!finished)
    {
#ifdef DEBUG
        Serial.println("[Info] Alarm is active!");
#endif
        return true;
    }
    else
    {
        // Holt sich die aktuelle Zeit in Minuten nach 00:00
        uint16_t current_time = get_minutes_passed();
        // Vergleicht die aktuellen Zeiten mit den Alarmzeiten
        for (uint8_t i = 0; i < alarm_count; i++)
        {
            if (!is_triggered(i))
            {
                if (current_time == alarms[i])
                {
                    // Flag um ein erneutes Läuten in der selben Minute zu verhindern

                    set_triggered(i);
                    current_alarm_type = alarms_type_assignment[i];
                    finished = false;
#ifdef DEBUG
                    Serial.print("[Info] Alarm triggered: ");
                    Serial.println(alarms[i]);
#endif
                    return true;
                }
            }
        }
        return false;
    }
}

uint32_t Time::get_ring_type(uint8_t index)
{
    return alarm_types[index];
}

void Time::set_alarm_types(uint8_t index, uint32_t ring_type)
{
    alarm_types[index] = ring_type;

    // ring_types[0] = 0xAAAAAAAA;
    // ring_types[1] = 0xF0F0F0F0;
    // ring_types[2] = 0xAA0F0AAA;
}

void Time::init_alarm_interrupt()
{

    // Reset Timer1
    TCCR1A = 0;

    // Reset timer on compare interrupt match
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);

    // Set prescaler to 64
    TCCR1B |= (1 << CS11);
    TCCR1B |= (1 << CS10);

    // Set compare register
    TCNT1 = 0;
    OCR1A = 62500;

    // Enable Timer1 compare interrupt ~ interrupts every 0.25 seconds
    TIMSK1 = (1 << OCIE1A);

    // Enable global interrupts
    interrupts();
}

ISR(TIMER1_COMPA_vect)
{
    // Wird abgefragt um ungewolltes Läuten zu vermeiden
    if (!finished)
    {
        if (alarm_types[current_alarm_type] & (1ul << position))
        {
            // HIGH wenn bit von ring_types an position 1 ist.
            analogWrite(A2, 1023);
        }
        else
        {
            // LOW wenn bit von ring_types an position 0 ist.
            analogWrite(A2, LOW);
        }
        if (position >= 32)
        {
            // Nach 8 Sekunden ist das Läuten fertig
            finished = true;
            position = 0;
        }
        else
        {
            // Position wird für den nächsten left-shift um 1 erhöht
            position += 1;
        }
    }
}