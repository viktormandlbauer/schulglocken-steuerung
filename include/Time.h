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

    bool check_alarm(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t alarm_count);

    uint8_t add_alarm(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t alarm_count, uint8_t hour, uint8_t minute, uint8_t alarm_type);

    uint8_t remove_alarm_at_index(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t alarm_count, uint8_t index);

    void get_alarms_strings(uint16_t alarms[], uint8_t alarm_count, char output[][6]);

    void init_alarm_interrupt();

    void set_alarm_types(uint8_t index, uint32_t ring_type);

    uint32_t get_ring_type(uint8_t index);

    void get_alarm_string(uint16_t alarm, char output[6]);
}
