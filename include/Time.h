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
    
    // Liefert das aktuelle Datum und die Uhrzeit als char array zurück
    char *get_time_string();

    // Verwendet minute()
    bool check_alarm();

    // Aktiviert die Schuglocke 
    bool ring(uint8_t alarm_type);

}
