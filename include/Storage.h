// 1024 bytes on the ATmega328P
// 4 KB (4096 bytes) on the ATmega1280 and ATmega2560

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
}