// Einstellung für den DEBUG Mode
#include "DEFINITIONS.h"
#ifdef DEBUG_STORAGE
#define DEBUG
#endif

#include <Arduino.h>
#include <EEPROM.h>

// Angabe des Header Files
#include "Storage.h"

// Verwendung des Namespaces "Storage" aus der Storage.h
using namespace Storage;

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
    /**
     * Speichern der Alarme und die Alarmtypzuweisung vom RAM in den EEPROM.
     * Maximal 64 Alarme sind möglich.
    */

#ifdef DEBUG
    Serial.print("[Info] (Storage) Anzahl der Alarme: ");
    Serial.println(count);
#endif

    // Speichere Anzahl der Alarme
    EEPROM.put(ALARMS_START, count);

    // Speichere Alarme
    for (int i = 0; i < count; i++)
    {

#ifdef DEBUG
        Serial.print("[Info] (Storage) Alarm wird gespeichert: ");
        Serial.println(alarms[i]);
        Serial.print("[Info] (Storage) Alarmtyp wird gespeichert: ");
        Serial.println(alarms_type_assignment[i]);
#endif

        EEPROM.put(1 + ALARMS_START + sizeof(uint16_t) * i, alarms[i]);
        EEPROM.put(ALARMS_ASSIGNMENT_START + sizeof(uint8_t) * i, alarms_type_assignment[i]);
    }
}

uint8_t Storage::read_alarms(uint16_t *alarms, uint8_t *alarms_type_assignment)
{
    /**
     * Auslesen der gespeicherten Alarme aus dem EEPROM.
     * Der Rückgabewert ist die Anzahl der gespeicherten Alarme.
    */

    uint8_t count = EEPROM.read(ALARMS_START);

#ifdef DEBUG
    Serial.print("[Info] (Storage) Anzahl der Alarme: ");
    Serial.println(count);
#endif

    for (int i = 0; i < count; i++)
    {
        EEPROM.get(1 + ALARMS_START + sizeof(uint16_t) * i, alarms[i]);
        EEPROM.get(ALARMS_ASSIGNMENT_START + sizeof(uint8_t) * i, alarms_type_assignment[i]);

#ifdef DEBUG
        Serial.print("[Info] (Storage) Alarm wurde ausgelesen: ");
        Serial.println(alarms[i]);
        Serial.print("[Info] (Storage) Alarmtyp wurde ausgelesen: ");
        Serial.println(alarms_type_assignment[i]);
#endif
    }

    return count;
}

void Storage::save_alarm_types(uint64_t *alarm_types, uint8_t count)
{
    EEPROM.put(ALARM_TYPES_START, count);

    for (int i = 0; i < count; i++)
    {
        EEPROM.put(1 + ALARM_TYPES_START + sizeof(uint64_t) * i, alarm_types[i]);
    }
}

uint8_t Storage::read_alarm_types(uint64_t *alarm_types)
{
    uint8_t count = EEPROM.read(ALARM_TYPES_START);

    for (int i = 0; i < count; i++)
    {
        alarm_types[i] = EEPROM.read(1 + ALARM_TYPES_START + sizeof(uint64_t));
    }

    return count;
}

void Storage::save_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);
uint8_t Storage::read_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);

void Storage::save_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
uint8_t Storage::read_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
