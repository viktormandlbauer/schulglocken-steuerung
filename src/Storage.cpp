#include "Storage.h"

/*
    Address 0: Alarm count

    sizeof(uint16_t) * 64 -> 256 Byte
    sizeof(uint8_t) * 64 ->  64 Byte
    sizeof(uint64_t) * 4 -> 250 Byte

*/

#define ALARMS_START 0
#define ALARMS_ASSIGNMENT_START 258
#define ALARM_TYPES_START 322

// 578 is next

void Storage::save_alarms(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t count)
{
    // Speichere Anzahl der Alarme
    EEPROM.put(ALARMS_START, count);

    // Speichere Alarme
    for (int i = 0; i <= count; i++)
    {
        EEPROM.put(1 + ALARMS_START + sizeof(uint16_t) * i, alarms[i]);
        EEPROM.put(ALARMS_ASSIGNMENT_START + sizeof(uint8_t) * i, alarms_type_assignment[i]);
    }
}

uint8_t Storage::read_alarms(uint16_t *alarms, uint8_t *alarms_type_assignment)
{
    uint8_t count = EEPROM.read(ALARMS_START);
    for (int i = 0; i <= count; i++)
    {
        alarms[i] = EEPROM.read(ALARMS_START + i * sizeof(uint16_t));
        alarms_type_assignment[i] = EEPROM.read(ALARMS_ASSIGNMENT_START + i * sizeof(uint8_t));
    }

    return count;
}

void Storage::save_alarm_types(uint64_t *alarm_types, uint8_t count)
{
    EEPROM.put(ALARM_TYPES_START, count);

    for (int i = 0; i <= count; i++)
    {
        EEPROM.put(1 + ALARM_TYPES_START + sizeof(uint64_t) * i, alarm_types[i]);
    }
}

uint8_t Storage::read_alarm_types(uint64_t *alarm_types)
{
    uint8_t count = EEPROM.read(ALARM_TYPES_START);

    for (int i = 0; i <= count; i++)
    {
        alarm_types[i] = EEPROM.read(1 + ALARM_TYPES_START + sizeof(uint64_t));
    }

    return count;
}

void Storage::save_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);
uint8_t Storage::read_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);

void Storage::save_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
uint8_t Storage::read_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
