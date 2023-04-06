#include "DEFINITIONS.h"
#ifdef DEBUG_STORAGE
#define DEBUG
#endif

#include <Arduino.h>
#include <EEPROM.h>

namespace Storage
{
    uint8_t read_alarm_count();
    void read_alarm(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t index);

    void save_alarm_count(uint8_t alarm_count);
    void save_alarm(uint16_t alarm, uint8_t alarms_type_assignment, uint8_t index);

    void save_alarm_types(uint64_t *alarm_types, uint8_t count);
    uint8_t read_alarm_types(uint64_t *alarm_types);

    void save_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t prefix);
    void read_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t *prefix);

    void save_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
    void read_network_time_settings(char *myntp_name, uint8_t *myntp_adr);

    void save_network_dhcp(bool automatic);
    bool read_network_dhcp();
}