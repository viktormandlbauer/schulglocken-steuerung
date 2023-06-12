// Angabe des Header Files
#include "Time.h"
#include "TimeSync.h"

// Array mit Alarmtypen
uint32_t alarm_types[MAXIMUM_AMOUNT_ALARM_TYPES] = {ALARM_TYPES};

uint8_t current_alarm_type = 0;

uint32_t position;
bool finished = true;
char time_internal_timestring[9];

namespace Time
{
    RTC_DS3231 rtc;

    // Aktuelle Zeit als Zeichenkette im Format DD.MM.YYYY HH:MM:SS
    // Rückgabewert "true", wenn Zeitänderung
    uint8_t last_second = 0;
    bool get_current_datetime(char *buffer)
    {
        uint8_t current_second = second();
        if (last_second != current_second)
        {
            last_second = current_second;
            sprintf(buffer, "%02d:%02d:%02d %02d.%02d.%04d", hour(), minute(), current_second, day(), month(), year());
            return true;
        }
        return false;
    }

    void get_current_date(char *buffer)
    {
        sprintf(buffer, "%02d.%02d.%04d", day(), month(), year());
    }

    void get_alarmstring(int hour, int minute, char buf[6])
    {
        sprintf(buf, "%02d:%02d", hour, minute);
    }

    // Interne Funtkion für die Konvertierung von Stunden und Minuten in vergehende Minuten von 00:00:00 weg
    uint16_t convert_time_to_alarm(uint8_t hour, uint8_t minute)
    {
        return hour * 60ul + minute;
    }

    void get_formatted_datetime(time_t unix_time, char *buffer)
    {
        sprintf(buffer, "%02d.%02d.%04d %02d:%02d:%02d", day(unix_time), month(unix_time), year(unix_time), hour(unix_time), minute(unix_time), second(unix_time));
    }

    void get_current_weekday(char *buffer)
    {
        uint8_t number_weekday = weekday();
        char weekdays[7][4] = {"Mo.", "Di.", "Mi.", "Do.", "Fr.", "Sa.", "So."};
        strcpy(buffer, weekdays[number_weekday]);
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

    bool init_rtc_module()
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

    uint8_t dstOffset(uint8_t d, uint8_t m, uint16_t y, uint8_t h)
    {
        // Berechnung des Datums, an dem die Sommerzeit beginnt und endet
        uint8_t dstOn = (31 - (5 * y / 4 + 4) % 7);
        uint8_t dstOff = (31 - (5 * y / 4 + 1) % 7);

        // Überprüfung, ob das aktuelle Datum innerhalb des Sommerzeitzeitraums liegt
        if ((m > 3 && m < 10) ||
            (m == 3 && (d > dstOn || (d == dstOn && h >= 1))) ||
            (m == 10 && (d < dstOff || (d == dstOff && h <= 1))))
            return 1;
        else
            return 0;
    }
    
    uint8_t utcOffset = 1;
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

        rtc.adjust(DateTime(new_year, new_month, new_day, new_hour, new_minute, new_second));
        setTime(new_hour, new_minute, new_second, new_day, new_month, new_year);
        // now();
#ifdef DEBUG
        Serial.print(F("[Info] (Time) Datetime set: "));
        Serial.print(new_hour);
        Serial.print(F(":"));
        Serial.print(new_minute);
        Serial.print(F(":"));
        Serial.print(new_second);
        Serial.print(" ");
        Serial.print(new_day);
        Serial.print(".");
        Serial.print(new_month);
        Serial.print(".");
        Serial.println(new_year);
#endif
    }

    void set_datetime(char datetime_buffer[20])
    {
        uint8_t hour, minute, second, day, month;
        uint16_t year;
        sscanf(datetime_buffer, "%" SCNu8 ":%" SCNu8 ":%" SCNu8 " %" SCNu8 ".%" SCNu8 ".%" SCNu16, &hour, &minute, &second, &day, &month, &year);
        set_datetime(year, month, day, hour, minute, second);
    }

    void get_alarm_string(uint16_t alarm, char output[6])
    {
        get_alarmstring(alarm / 60, alarm % 60, output);
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
            get_alarmstring(alarms[i].minutes / 60, alarms[i].minutes % 60, output[i]);
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

    void alarm_exception_to_string(AlarmException alarm_exception, char buffer[13])
    {

        sprintf(buffer, "%02d.%02d-%02d.%02d", alarm_exception.BeginDay, alarm_exception.BeginMonth, alarm_exception.EndDay, alarm_exception.EndMonth);
    }

    void get_alarm_exceptions_strings(AlarmException *alarm_exceptions, uint8_t alarm_exception_count, char buffer[][13], bool reoccurring_exception[])
    {
        for (uint8_t i = 0; i < alarm_exception_count; i++)
        {
            alarm_exception_to_string(alarm_exceptions[i], buffer[i]);
            reoccurring_exception[i] = alarm_exceptions[i].reoccurring;
        }
    }

    AlarmException parse_to_alarm_exception(char exception_start[6], char exception_end[6], bool reoccurring)
    {
        AlarmException alarm_exception;
        sscanf(exception_start, "%hhu.%hhu", &alarm_exception.BeginDay, &alarm_exception.BeginMonth);
        sscanf(exception_end, "%hhu.%hhu", &alarm_exception.EndDay, &alarm_exception.EndMonth);
        alarm_exception.reoccurring = reoccurring;
        return alarm_exception;
    }

    uint8_t add_alarm_exception(AlarmException *alarm_exceptions, AlarmException alarm_exception, uint8_t alarm_exceptions_count)
    {
#ifdef DEBUG
        Serial.print("[Info] (Time) Adding alarm exception: ");
        Serial.print(alarm_exception.BeginDay);
        Serial.print(".");
        Serial.print(alarm_exception.BeginMonth);
        Serial.print(" - ");
        Serial.print(alarm_exception.EndDay);
        Serial.print(".");
        Serial.print(alarm_exception.EndMonth);
        Serial.print(" - Reoccurring: ");
        Serial.println(alarm_exception.reoccurring);
#endif
        alarm_exceptions[alarm_exceptions_count] = alarm_exception;
        return alarm_exceptions_count + 1;
    }

    uint8_t remove_alarm_exception_at_index(AlarmException *alarm_exceptions, uint8_t exception_count, uint8_t index)
    {
        for (uint8_t i = index; i < exception_count; i++)
        {
            alarm_exceptions[i] = alarm_exceptions[i + 1];
        }
        return exception_count - 1;
    }

    void get_upcoming_alarm_strings(Alarm alarms[], uint8_t alarm_count, char output[][6], uint8_t result_count)
    {
        uint16_t minutes_passed = get_minutes_passed();
        uint8_t found = 0;
        for (uint8_t i = 0; (i < alarm_count) && (found < result_count); i++)
        {
            if (minutes_passed < alarms[i].minutes)
            {
                get_alarmstring(alarms[i].minutes / 60, alarms[i].minutes % 60, output[found]);
                found += 1;
            }
        }
    }

    struct Date
    {
        uint8_t day;
        uint8_t month;
    };

    bool in_daterange(const AlarmException &alarm_exception, const Date &current_date)
    {
        const uint8_t months_in_year = 12;

        if (current_date.month >= alarm_exception.BeginMonth && current_date.month <= alarm_exception.EndMonth)
        {
            // Jahressprung
            if (alarm_exception.BeginMonth > alarm_exception.EndMonth)
            {
                uint8_t month_diff = (alarm_exception.EndMonth + months_in_year) - alarm_exception.BeginMonth;
                uint8_t current_diff = current_date.month - alarm_exception.BeginMonth;

                if (current_diff < month_diff ||
                    (current_diff == 0 && current_date.day >= alarm_exception.BeginDay) ||
                    (current_diff == month_diff && current_date.day <= alarm_exception.EndDay))
                {
                    return true;
                }
            }
            // Ohne Jahressprung
            else
            {
                if ((current_date.month > alarm_exception.BeginMonth && current_date.month < alarm_exception.EndMonth) ||
                    (current_date.month == alarm_exception.BeginMonth && current_date.day >= alarm_exception.BeginDay && current_date.day <= alarm_exception.EndDay) ||
                    (current_date.month == alarm_exception.EndMonth && current_date.day >= alarm_exception.BeginDay && current_date.day <= alarm_exception.EndDay))
                {
                    return true;
                }
            }
        }
        return false;
    }

    void reset_alarms(Alarm *alarms, uint8_t alarm_count)
    {
        for (uint8_t i = 0; i < alarm_count; i++)
        {
            alarms[i].triggered = false;
        }
    }

    bool check_alarm_exception(AlarmException *alarm_exceptions, uint8_t alarm_exceptions_count, uint8_t weekday_exception_list)
    {
        if (!(weekday_exception_list & (0b1 << (weekday() - 1))))
        {
#ifdef DEBUG
            Serial.println("[Info] (Time) Weekday exception is active!");
#endif
            return true;
        }

        Date current_date;
        current_date.day = day();
        current_date.month = month();

        for (uint8_t i = 0; i < alarm_exceptions_count; i++)
        {
            if (in_daterange(alarm_exceptions[i], current_date))
            {
#ifdef DEBUG
                Serial.println("[Info] (Time) Date exception is active!");
#endif
                return true;
            }
        }
        return false;
    }

    // Variable auf 255 gesetzt, damit beim Start getriggered wird
    uint8_t last_day = 255;
    bool exceptions_checked = false;

    bool day_changed()
    {
        // Wenn es einen Tagessprung gibt, müssen alle Alarme scharf geschalten und die Ausnahmen neu geprüft werden.
        uint8_t current_day = day();
        if (current_day != last_day)
        {
            last_day = current_day;
            return true;
        }
        else
        {
            return false;
        }
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
                if (!alarms[i].triggered)
                {
                    if (current_time == alarms[i].minutes)
                    {
                        // Flag um ein erneutes Läuten in der selben Minute zu verhindern
                        alarms[i].triggered = true;
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
    }

    void init_alarm_interrupt()
    {
        // Zurücksetzen von Timer/Counter1 Control Register A
        TCCR1A = 0;

        // Setze CTC (Clear Timer on Compare) Modus in Timer/Counter Control Register B
        TCCR1B &= ~(1 << WGM13);
        TCCR1B |= (1 << WGM12);

        // Setze Prescaler auf 64 in Timer/Counter1 in Controler Register B
        TCCR1B |= (1 << CS11);
        TCCR1B |= (1 << CS10);

        // Setze Timer/Counter1 auf 0
        TCNT1 = 0;

        // Setze Output Compare Register
        OCR1A = 62500;

        // Aktiviere Vergleichsregister für Timer/Counter1 
        TIMSK1 = (1 << OCIE1A);

        // Aktiviere globale Interrupts
        interrupts();
    }
}

ISR(TIMER1_COMPA_vect)
{

    // Abfrage um Läuten zu vermeiden
    if (!finished)
    {
        if (alarm_types[current_alarm_type] & (1ul << position))
        {
            // HIGH, wenn Bit an aktueller position 1 ist.
            digitalWrite(LED_SIGNAL, HIGH);
            digitalWrite(RELAIS, HIGH);
#ifdef DEBUG
            digitalWrite(AUDIO_SIGNAL, HIGH);
#endif
        }
        else
        {
            // LOW, wenn Bit an aktueller position 0 ist
            digitalWrite(LED_SIGNAL, LOW);
            digitalWrite(RELAIS, LOW);
#ifdef DEBUG
            digitalWrite(AUDIO_SIGNAL, LOW);
#endif
        }
        if (position >= 32)
        {
            // Nach 8 Sekunden ist das Läuten fertig
            digitalWrite(RELAIS, LOW);
            digitalWrite(LED_SIGNAL, LOW);
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