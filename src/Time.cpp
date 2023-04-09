// Angabe des Header Files
#include "Time.h"
#include "TimeSync.h"

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

// Funktionen zur Überprüfung des Status eines Alarms
bool is_triggered(uint8_t index) { return triggered & (0b1 << index); }
void set_triggered(uint8_t index) { triggered |= 0b1 << index; }

uint8_t current_alarm_type;
uint32_t position;
bool finished = true;
char time_internal_timestring[9];

void get_timestring(int hour, int minute, char *buf)
{
    /**
     * Formatierung "HH:MM" Format
     */

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

// Interne Funtkion für die Konvertierung von Stunden und Minuten in vergehende Minuten von 00:00:00 weg
uint16_t convert_time_to_alarm(uint8_t hour, uint8_t minute)
{
    return hour * 60ul + minute;
}

namespace Time
{

    RTC_DS3231 rtc;

    // Funktion zur Formatierung eines Unix-Zeitstempels als Zeichenkette im Format DD.MM.YYYY HH:MM:SS
    void get_formatted_time(time_t unix_time, char *buffer)
    {
        sprintf(buffer, "%02d.%02d.%04d %02d:%02d:%02d", day(unix_time), month(unix_time), year(unix_time), hour(unix_time), minute(unix_time), second(unix_time));
    }

    int compare(const void *s1, const void *s2)
    {
        struct Alarm *a1 = (struct Alarm *)s1;
        struct Alarm *a2 = (struct Alarm *)s2;

        if (a1->minutes < a2->minutes)
        {
            return -1;
        }
        else if (a1->minutes > a2->minutes)
        {
            return +1;
        }
        else
        {
            return 0;
        }
    }

    bool init_rtc_module(void)
    {
        while (!rtc.begin())
        {
            position = +1;
            if (position < 600)
            {
                delay(100);
            }
            else
            {
                position = 0;
                return false;
            }
        }
        position = 0;
        return true;
    }

    byte dstOffset(byte d, byte m, unsigned int y, byte h)
    {
        // Berechnung des Datums, an dem die Sommerzeit beginnt und endet
        byte dstOn = (31 - (5 * y / 4 + 4) % 7);
        byte dstOff = (31 - (5 * y / 4 + 1) % 7);

        // Überprüfung, ob das aktuelle Datum innerhalb des Sommerzeitzeitraums liegt
        if ((m > 3 && m < 10) ||
            (m == 3 && (d > dstOn || (d == dstOn && h >= 1))) ||
            (m == 10 && (d < dstOff || (d == dstOff && h <= 1))))
            return 1;
        else
            return 0;
    }

    time_t get_corrected_utctime(time_t t)
    {
        // Überprüfen, ob die Eingabezeit gültig ist
        if (t > 0)
        {
            // Eingabezeit in eine "TimeElements"-Struktur umwandeln
            TimeElements tm;
            breakTime(t, tm);
            // Die UTC- und DST-Offsetwerte zur Eingabezeit addieren
            t += (utcOffset + dstOffset(tm.Day, tm.Month, tm.Year + 1970, tm.Hour)) * SECS_PER_HOUR;
        }

        // Die korrigierte Zeit zurückgeben
        return t;
    }

    bool get_current_timestring(char output[9])
    {
        // Schreibt die in "HH:MM:SS" formatierte Zeit in ein character array der länge 9
        sprintf(output, "%02d:%02d:%02d", hour(), minute(), second());

        if (strcmp(output, time_internal_timestring) != 0)
        {
#ifdef DEBUG
            Serial.println(F("[Info] (Time) HH:MM:SS changed"));
#endif
            memcpy(time_internal_timestring, output, 9 * sizeof(*output));
            return true;
        }
        return false;
    }

    void set_datetime(uint16_t new_year, uint8_t new_month, uint8_t new_day, uint8_t new_hour, uint8_t new_minute, uint8_t new_second)
    {
        // rtc.adjust(DateTime(new_year, new_month, new_day, new_hour, new_minute, new_second));
        rtc.adjust(DateTime(new_year, new_month, new_day, new_hour, new_minute, new_second));
        setTime(new_hour, new_minute, new_second, new_day, new_month, new_year);
        now();
#ifdef DEBUG
        Serial.print(F("[Info] (Time) Time set: "));
        Serial.print(new_hour);
        Serial.print(F(":"));
        Serial.print(new_minute);
        Serial.print(F(":"));
        Serial.println(new_second);
#endif
    }

    void timestring_to_timearray(char *time, uint8_t datetime_array[3])
    {
        datetime_array[0] = (time[0] - '0') * 10 + (time[1] - '0');
        datetime_array[1] = (time[3] - '0') * 10 + (time[4] - '0');
        datetime_array[2] = (time[6] - '0') * 10 + (time[7] - '0');
    }

    void get_alarm_string(uint16_t alarm, char output[6])
    {
        get_timestring(alarm / 60, alarm % 60, output);
    }

    void sort_alarms(Alarm alarms[], uint8_t alarm_count)
    {

        qsort(alarms, alarm_count, sizeof(Alarm), compare);
    }

    bool alarm_exists(Alarm alarms[], uint16_t minutes, uint8_t alarm_count)
    {
        for (uint8_t i = 0; i < alarm_count; i++)
        {
            if (alarms[i].minutes == minutes)
            {
                return true;
            }
        }
        return false;
    }

    bool add_alarm(Alarm alarms[], uint8_t *alarm_count, uint16_t minutes, uint8_t alarm_type)
    {
        if (!alarm_exists(alarms, minutes, *alarm_count))
        {
            alarms[*alarm_count].minutes = minutes;
            alarms[*alarm_count].type = alarm_type;
            *alarm_count += 1;
#ifdef DEBUG
            Serial.print(F("[Info] (Time) Added new alarm "));
            Serial.print(minutes);
            Serial.print(F(" with type: "));
            Serial.println(alarm_type);
            Serial.print(F("[Info] (Time) Alarm count: "));
            Serial.println(*alarm_count);
#endif
        }
        else
        {
#ifdef DEBUG
            Serial.println(F("[Error] (Time) Alarm already exists"));
#endif
            return false;
        }

        return true;
    }

    uint8_t remove_alarm_at_index(Alarm alarms[], uint8_t alarm_count, uint8_t index)
    {
        for (int i = index; i < alarm_count - 1; i++)
        {
            alarms[i] = alarms[i + 1];
        }
#ifdef DEBUG
        Serial.print(F("[Info] (GUI) Removed alarm at index: "));
        Serial.println(index);
        Serial.print(F("[Info] (GUI) New alarm count: "));
        Serial.println(alarm_count - 1);
#endif

        return alarm_count - 1;
    }

    void get_alarms_strings(Alarm alarms[], uint8_t alarm_count, char output[][6])
    {

        // Füllt ein zweidimensionales character array mit den formatierten Alarmzeiten und gibt die Anzahl der Alarme zurück.
        for (int i = 0; i < alarm_count; i++)
        {
            get_timestring(alarms[i].minutes / 60, alarms[i].minutes % 60, output[i]);
        }
    }

    /*
    Setter und Getter für Alarme
    */

    uint16_t get_minutes_passed()
    {
        // Gibt die Minuten nach 00:00 als Ganzzahl zurück
        return convert_time_to_alarm(hour(), minute());
    }

    // Allgemeine Überprüfung eines Alarms
    bool check_alarms(Alarm *alarms, uint8_t alarm_count)
    {
        // Testet ob Alarm aktiv ist
        if (!finished)
        {
#ifdef DEBUG
            Serial.println(F("[Info] Alarm is active!"));
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
                    if (current_time == alarms[i].minutes)
                    {
                        // Flag um ein erneutes Läuten in der selben Minute zu verhindern

                        set_triggered(i);
                        current_alarm_type = alarms[i].type;
                        finished = false;
#ifdef DEBUG
                        Serial.print(F("[Info] Alarm triggered: "));
                        Serial.println(alarms[i].minutes);
#endif
                        return true;
                    }
                }
            }
            return false;
        }
    }

    uint32_t get_ring_type(uint8_t index)
    {
        return alarm_types[index];
    }

    void set_alarm_types(uint8_t index, uint32_t ring_type)
    {
        alarm_types[index] = ring_type;

        // ring_types[0] = 0xAAAAAAAA;
        // ring_types[1] = 0xF0F0F0F0;
        // ring_types[2] = 0xAA0F0AAA;
    }

    void init_alarm_interrupt()
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
}

ISR(TIMER1_COMPA_vect)
{
    // Wird abgefragt um ungewolltes Läuten zu vermeiden
    if (!finished)
    {
        if (alarm_types[current_alarm_type] & (1ul << position))
        {
            // HIGH wenn bit von ring_types an position 1 ist.
            digitalWrite(OUTPUT_PIN, HIGH);
        }
        else
        {
            // LOW wenn bit von ring_types an position 0 ist.
            digitalWrite(OUTPUT_PIN, LOW);
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