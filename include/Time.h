#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <RTClib.h>
#include <TimeLib.h>

namespace Time
{
    // RTC Objekt aus RTClib.h für die Verwendung in der Zeitsynchronisation
    extern RTC_DS3231 rtc;

    // Stuktur für Alarme
    struct Alarm
    {
        uint16_t minutes;
        uint8_t type;
        bool triggered;
    };

    // Struktur für Alarmausnahmen
    struct AlarmException
    {
        uint8_t BeginDay, BeginMonth, EndDay, EndMonth;
        uint16_t Days;
        bool reoccurring;
    };

    // Initialisierung der Real Time Clock
    bool init_rtc_module();

    // Initialisierung für den Timer Interrupt für das Läuten
    void init_alarm_interrupt();

    // Überprüft ob es seit der letzten Abfrage eine Tagesänderung gibt
    bool day_changed();

    // Gibt die korrigierte UTC Zeit zurück (Anpassung an die Sommerzeit)
    time_t get_corrected_utctime(time_t t);

    // Gibt die formatierten Unix Zeit als Zeichenkette im Format DD.MM.YYYY HH:MM:SS zurück
    void get_formatted_datetime(time_t unix_time, char buffer[20]);

    // Gibt das aktuelle Datum zurück im Format DD.MM.YYYY zurück
    void get_current_date(char buffer[11]);

    // Gibt die aktuelle Uhrzeit im Format HH:MM:SS zurück
    bool get_current_timestring(char time_string[9]);

    // Gibt aktuelle Zeit und Datum im Format DD.MM.YYYY HH:MM:SS zurück
    bool get_current_datetime(char buffer[20]);

    // Gibt die aktuelle Wochentagabkürzung (Mo. , Di. , ..) zurück
    void get_current_weekday(char buffer[4]);

    // Setzt die System- und RTC-Zeit anhand von Zahlendatentypen
    void set_datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

    // Setzt die System und RTC-Zeit anhand einer Zeichenkette im Format DD.MM.YYYY HH:MM:SS
    void set_datetime(char datetime_buffer[20]);

    // Überprüft ob zur aktuellen Minute eine Alarm aktiv geschalten wird
    bool check_alarms(Alarm alarms[], uint8_t alarm_count);

    // Fügt einen Alarm hinzu
    bool add_alarm(Alarm alarms[], uint8_t *alarm_count, uint16_t minutes, uint8_t alarm_type);

    // Überprüft ob ein Alarm bereits existiert
    bool alarm_exists(Alarm alarms[], uint16_t minutes, uint8_t alarm_count);

    // Sortiert die Alarme im übergebenen Array
    void sort_alarms(Alarm alarms[], uint8_t alarm_count);

    // Entfernt den Alarm anhand des übergebenen Index aus dem übergebenen Array
    uint8_t remove_alarm_at_index(Alarm alarms[], uint8_t alarm_count, uint8_t index);

    // Setzt den "Ausgelöst" Status aller Alarme auf falsch
    void reset_alarms(Alarm alarms[], uint8_t alarm_count);

    // Schreibte alle Alarme in das Zeichenketten Array
    void get_alarms_strings(Alarm alarms[], uint8_t alarm_count, char buffer[][6]);

    // Gibt einen einzelnen Alarm als Zeichenkette im Format HH:MM zurück
    void get_alarm_string(uint16_t alarm, char buffer[6]);

    // Schreibt die anstehenden Alarme als Zeichenkette in das Array
    void get_upcoming_alarm_strings(Alarm alarms[], uint8_t alarm_count, char buffer[][6], uint8_t result_count);

    // Überprüft ob eine Alarmausnahme für den Tag aktiv ist
    bool check_alarm_exception(AlarmException alarm_exceptions[], uint8_t alarm_exceptions_count, uint8_t weekday_exception_list);

    // Fügt eine Alarmausnahme hinzu
    uint8_t add_alarm_exception(AlarmException alarm_exceptions[], AlarmException alarm_exception, uint8_t alarm_exceptions_count);

    // Entfernt die Alarmausnahme anhand des übergebenen Index aus dem übergebenen Array
    uint8_t remove_alarm_exception_at_index(AlarmException alarm_excpetions[], uint8_t exception_count, uint8_t index);

    // Gibt anhand von Zeichenketten eine Alarmausnahme zurück
    AlarmException parse_to_alarm_exception(char exception_start[6], char exception_end[6], bool reoccurring);

    // Schreibt alle Alarmausnahmen in das Zeichenketten Array
    void get_alarm_exceptions_strings(AlarmException alarm_exceptions[], uint8_t alarm_exception_count, char buffer[][13], bool *reoccurring_exception);
}
