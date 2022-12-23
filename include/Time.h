#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <TimeLib.h>

// Dokumentation der Zeit
/**
 * Alerts auf Tagesbasis
 * Ausnahmen auf Wochentagbasis (Mo - So)
 * Ausnahmen Datumsbasiert (einmalig)
 */

// Format der Zeit
/**
 * Zwei statische arrays[100] :
 *   * uint16_t Minute
 *   * bool beginn
 */
namespace Time
{
    bool init_rtc_module();

    // Schreibt die Zeit in "HH:MM:SS" Format in ein char Array
    // char time_string[9];
    void get_time(char *time_string);

    uint16_t get_minutes_passed();

    // Verwendet minute()
    bool check_alarm();

    // Aktiviert die Schuglocke
    bool ring(uint8_t alarm_type);

    uint8_t add_alarm(uint8_t hour, uint8_t minute, uint8_t alarm_type);
    uint8_t remove_alarm(uint8_t hour, uint8_t minute);
}
