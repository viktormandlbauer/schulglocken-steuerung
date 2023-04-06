// Einstellung für den DEBUG Mode
#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif


// Angabe des Header Files
#include "Time.h"

// RTC Objekt aus der RTClib
RTC_DS3231 rtc;

// Verwendung des Namespaces
using namespace Time;

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

// Intialisierung der Zeitzonenregeln (DEFINITIONS.h)
TimeChangeRule myDST = DST;
TimeChangeRule mySTD = STD;
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr;

time_t time_provider()
{
    time_t utc = rtc.now().unixtime();
    time_t local = myTZ.toLocal(utc, &tcr);
    return local;
}

bool Time::init_rtc_module()
{
    /**
     * Initiierung des RTC Moduls
     */

    while (!rtc.begin())
        ;
}

void get_timestring(int hour, int minute, int second, char *buf)
{
    /**
     * Formatierung "HH:MM:SS" Format
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

uint16_t convert_time_to_alarm(uint8_t hour, uint8_t minute)
{
    // Interne Funtkion für die Konvertierung von Stunden und Minuten in vergehende Minuten von 00:00:00 weg
    return hour * 60ul + minute;
}

char time_internal_timestring[9];

bool Time::get_current_timestring(char output[9])
{
    // Schreibt die in "HH:MM:SS" formatierte Zeit in ein character array der länge 9
    get_timestring(hour(), minute(), second(), output);

    if (strcmp(output, time_internal_timestring) != 0)
    {
#ifdef DEBUG
        Serial.println("[Info] (Time) HH:MM:SS changed");
#endif
        memcpy(time_internal_timestring, output, 9 * sizeof(*output));
        return true;
    }
    return false;
}

void Time::set_datetime(uint16_t new_year, uint8_t new_month, uint8_t new_day, uint8_t new_hour, uint8_t new_minute, uint8_t new_second)
{
    // rtc.adjust(DateTime(new_year, new_month, new_day, new_hour, new_minute, new_second));
    rtc.adjust(DateTime(new_year, new_month, new_day, new_hour, new_minute, new_second));
    setTime(new_hour, new_minute, new_second, new_day, new_month, new_year);
    now();
#ifdef DEBUG
    Serial.print("[Info] (Time) Time set: ");
    Serial.print(new_hour);
    Serial.print(":");
    Serial.print(new_minute);
    Serial.print(":");
    Serial.println(new_second);
#endif
}

void Time::timestring_to_timearray(char *time, uint8_t datetime_array[3])
{
    datetime_array[0] = (time[0] - '0') * 10 + (time[1] - '0');
    datetime_array[1] = (time[3] - '0') * 10 + (time[4] - '0');
    datetime_array[2] = (time[6] - '0') * 10 + (time[7] - '0');
}

void Time::get_alarm_string(uint16_t alarm, char output[6])
{
    get_timestring(alarm / 60, alarm % 60, output);
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

void Time::sort_alarms(Alarm alarms[], uint8_t alarm_count)
{

    qsort(alarms, alarm_count, sizeof(Alarm), compare);
}

bool Time::alarm_exists(Alarm alarms[], uint16_t minutes, uint8_t alarm_count)
{
    Serial.println(minutes);
    for (uint8_t i = 0; i < alarm_count; i++)
    {
        Serial.println(alarms[i].minutes);
        if (alarms[i].minutes == minutes)
        {
            return true;
        }
    }
    return false;
}

bool Time::add_alarm(Alarm alarms[], uint8_t *alarm_count, uint16_t minutes, uint8_t alarm_type)
{
    if (!alarm_exists(alarms, minutes, *alarm_count))
    {
        alarms[*alarm_count].minutes = minutes;
        alarms[*alarm_count].type = alarm_type;
        *alarm_count += 1;
#ifdef DEBUG
        Serial.print("[Info] (Time) Added new alarm ");
        Serial.print(minutes);
        Serial.print(" with type: ");
        Serial.println(alarm_type);
        Serial.print("[Info] (Time) Alarm count: ");
        Serial.println(*alarm_count);
#endif
    }
    else
    {
#ifdef DEBUG
        Serial.println("[Error] (Time) Alarm already exists");
#endif
        return false;
    }

    return true;
}

uint8_t Time::remove_alarm_at_index(Alarm alarms[], uint8_t alarm_count, uint8_t index)
{
    for (int i = index; i < alarm_count - 1; i++)
    {
        alarms[i] = alarms[i + 1];
    }
#ifdef DEBUG
    Serial.print("[Info] (GUI) Removed alarm at index: ");
    Serial.println(index);
    Serial.print("[Info] (GUI) New alarm count: ");
    Serial.println(alarm_count - 1);
#endif

    return alarm_count - 1;
}

void Time::get_alarms_strings(Alarm alarms[], uint8_t alarm_count, char output[][6])
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

uint16_t Time::get_minutes_passed()
{
    // Gibt die Minuten nach 00:00 als Ganzzahl zurück
    return convert_time_to_alarm(hour(), minute());
}

// Funktionen zur Überprüfung des Status eines Alarms
bool is_triggered(uint8_t index) { return triggered & (0b1 << index); }
void set_triggered(uint8_t index) { triggered |= 0b1 << index; }

uint8_t current_alarm_type;
uint32_t position;
bool finished = true;

// Allgemeine Überprüfung eines Alarms
bool Time::check_alarms(Alarm *alarms, uint8_t alarm_count)
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
                if (current_time == alarms[i].minutes)
                {
                    // Flag um ein erneutes Läuten in der selben Minute zu verhindern

                    set_triggered(i);
                    current_alarm_type = alarms[i].type;
                    finished = false;
#ifdef DEBUG
                    Serial.print("[Info] Alarm triggered: ");
                    Serial.println(alarms[i].minutes);
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