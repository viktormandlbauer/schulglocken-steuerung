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
    void get_current_timestring(char time_string[9]);
    uint16_t get_minutes_passed();
    bool check_alarm();
    uint8_t add_alarm(uint8_t hour, uint8_t minute, uint8_t alarm_type);
    uint8_t remove_alarm_at_index(uint8_t index);
    uint8_t get_alarms_strings(char time_string[][6]);
    uint8_t get_alarm_count();
    uint16_t *get_alarms();
    void set_alarms(uint16_t *alarms);
    uint8_t *get_alarm_assignements();
    void set_alarm_count(uint8_t alarm_count);

    void init_alarm_interrupt();
    void set_alarm_types(uint8_t index, uint32_t ring_type);
    uint32_t get_ring_type(uint8_t index);
}
