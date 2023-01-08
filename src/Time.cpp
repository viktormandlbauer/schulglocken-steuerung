// Einstellung für den DEBUG Mode
#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

// Einbindung externer Libraries 
#include "RTClib.h"
#include <Wire.h>
#include <TimeLib.h>
#include <SPI.h>

// Angabe des Header-Files
#include "Time.h"

// Verwendung des Namespaces
using namespace Time;

// RTC Objekt aus der RTClib
RTC_DS3231 rtc;

// Anzahl der Alarme
uint8_t alarm_count;

// Array mit den Alarmzeiten
uint16_t alarms[64];

// Array mit den Alarmtypen
uint8_t type_alarms[64];

// 64 bit Wert für die Abhandlung der Alarme
uint64_t triggered;

// Anzahl der Ausnahmen
uint8_t exception_count;

// Array mit 16 Bit Werten für Ausnahme Beginn-Tag
uint16_t exception_date_begin[20];

// Aray mit 16 Bit Werten für Ausnahme End-Tag
uint16_t exception_date_end[20];

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
    type_alarms[alarm_count] = alarm_type;
    alarm_count += 1;
    return alarm_count;
}

uint8_t Time::remove_alarm_at_index(uint8_t index)
{
    // Entfernt einen Alarm und den Typ am bestimmten Index
    for (int i = index; i < alarm_count - 1; i++)
    {
        alarms[i] = alarms[i + 1];
        type_alarms[i] = type_alarms[i + 1];
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

time_t alarm_start_time = 0;
time_t alarm_end_time = 0;

time_t get_alarm_end_time()
{
    //  5 Sekunden Alarm für Tests
    // TODO: Verschiedene Arten von Alarmen (Rhytmus usw.)
    return alarm_start_time + 5;
}

// Funktionen zur Überprüfung des Status eines Alarms
bool is_triggered(uint8_t index) { return triggered & (0b1 << index); }
void set_triggered(uint8_t index) { triggered |= 0b1 << index; }

uint8_t current_alarm_type = 0;
uint32_t position = 0;
bool finished = true;

// Allgemeine Überprüfung eines Alarms
bool Time::check_alarm()
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
                    current_alarm_type = type_alarms[i];
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

uint32_t ring_types[3];

void Time::init_ring_types()
{
    // Only 3 defined ring types atm
    ring_types[0] = 0xAAAAAAAA;
    ring_types[1] = 0xF0F0F0F0;
    ring_types[2] = 0xAA0F0AAA;
}

void Time::init_alarm_interrupt()
{
    /*
    Maximale Läutzeit - 8 Sekunden
    32 bit: 32 x 250 Millisekunden definierbar
    Clockspeed 16000 Mhz
    Intervall: 0,0000000625 -> 62,5 ns

    Compare Register with prescaler of 64:
    (0,25/(1/16 000 000))/64 = 62 500
    */

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

    // Enable Timer1 compare interrupt
    TIMSK1 = (1 << OCIE1A);

    // Enable global interrupts
    interrupts();
}

ISR(TIMER1_COMPA_vect)
{
    // Wird abgefragt um ungewolltes Läuten zu vermeiden
    if (!finished)
    {
        if (ring_types[current_alarm_type] & (1ul << position))
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