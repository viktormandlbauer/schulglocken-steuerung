// 1024 bytes on the ATmega328P
// 4 KB (4096 bytes) on the ATmega1280 and ATmega2560

namespace Storage
{
    void save_alarms(uint16_t alarms[], uint8_t alarms_type_assignment[], uint8_t count);
    uint8_t read_alarms(uint16_t alarms[], uint8_t alarms_type_assignment[]);

    void save_alarm_types(uint64_t *alarm_types, uint8_t count);
    uint8_t read_alarm_types(uint64_t *alarm_types);

    void save_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);
    void read_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);

    void save_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
    void read_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
}