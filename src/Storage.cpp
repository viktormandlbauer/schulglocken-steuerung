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
#define ALARMS_ASSIGNMENT_START MAXIMUM_AMOUNT_ALARMS * 2 + 1 
#define ALARM_TYPES_START ALARMS_ASSIGNMENT_START + MAXIMUM_AMOUNT_ALARMS * 1 + 1 
#define NETWORK_CONFIG_START ALARM_TYPES_START + MAXIMUM_AMOUNT_ALARM_TYPES * 8 + 1
#define NEXT NETWORK_CONFIG_START + 36 + 1 // 36 Bytes -> 4 x 32bit IP Addressen + 20 chars für NTP Server Name

// #define ALARMS_ASSIGNMENT_START 258
// #define ALARM_TYPES_START 322

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
    /**
     * Speichern von Alarmtypen (64bit Werte)
     */

    // Anzahl wird an erster Stelle des Addressbereichs gespeichert.
    EEPROM.put(ALARM_TYPES_START, count);

    for (int i = 0; i < count; i++)
    {
        EEPROM.put(1 + ALARM_TYPES_START + sizeof(uint64_t) * i, alarm_types[i]);

#ifdef DEBUG
        Serial.print("[Info] (Storage) Alarmtyp wurde gespeichert.");
#endif
    }
}

uint8_t Storage::read_alarm_types(uint64_t *alarm_types)
{
    /**
     * Auslesen von Alarmtypen
     */

    uint8_t count = EEPROM.read(ALARM_TYPES_START);

    for (int i = 0; i < count; i++)
    {
        EEPROM.get(1 + ALARM_TYPES_START + sizeof(uint64_t), alarm_types[i]);
#ifdef DEBUG
        Serial.print("[Info] (Storage) Alarmtyp wurde ausgelesen.");
#endif
    }

    return count;
}

void Storage::save_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns)
{
    // 32 bit IP Address
    EEPROM.put(NETWORK_CONFIG_START, myip[0]);
    EEPROM.put(NETWORK_CONFIG_START + 1, myip[1]);
    EEPROM.put(NETWORK_CONFIG_START + 2, myip[2]);
    EEPROM.put(NETWORK_CONFIG_START + 3, myip[3]);

    // 32 bit Gateway Address
    EEPROM.put(NETWORK_CONFIG_START + 4, mygw[0]);
    EEPROM.put(NETWORK_CONFIG_START + 5, mygw[1]);
    EEPROM.put(NETWORK_CONFIG_START + 6, mygw[2]);
    EEPROM.put(NETWORK_CONFIG_START + 7, mygw[3]);

    // 32 bit DNS Server Address
    EEPROM.put(NETWORK_CONFIG_START + 8, mydns[0]);
    EEPROM.put(NETWORK_CONFIG_START + 9, mydns[1]);
    EEPROM.put(NETWORK_CONFIG_START + 10, mydns[2]);
    EEPROM.put(NETWORK_CONFIG_START + 11, mydns[3]);
}
void Storage::read_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns)
{
    // 32 bit IP Address
    myip[0] = EEPROM.read(NETWORK_CONFIG_START);
    myip[1] = EEPROM.read(NETWORK_CONFIG_START + 1);
    myip[2] = EEPROM.read(NETWORK_CONFIG_START + 2);
    myip[3] = EEPROM.read(NETWORK_CONFIG_START + 3);

    // 32 bit Gateway Address
    mygw[0] = EEPROM.read(NETWORK_CONFIG_START + 4);
    mygw[1] = EEPROM.read(NETWORK_CONFIG_START + 5);
    mygw[2] = EEPROM.read(NETWORK_CONFIG_START + 6);
    mygw[3] = EEPROM.read(NETWORK_CONFIG_START + 7);

    // 32 bit DNS Server Address
    mydns[0] = EEPROM.read(NETWORK_CONFIG_START + 8);
    mydns[1] = EEPROM.read(NETWORK_CONFIG_START + 9);
    mydns[2] = EEPROM.read(NETWORK_CONFIG_START + 10);
    mydns[3] = EEPROM.read(NETWORK_CONFIG_START + 11);
}

void Storage::save_network_time_settings(char *myntp_name, uint8_t *myntp_adr)
{

    // 32 bit NTP Server IP Address
    EEPROM.put(NETWORK_CONFIG_START + 12, myntp_adr[0]);
    EEPROM.put(NETWORK_CONFIG_START + 13, myntp_adr[1]);
    EEPROM.put(NETWORK_CONFIG_START + 14, myntp_adr[2]);
    EEPROM.put(NETWORK_CONFIG_START + 15, myntp_adr[3]);

    // Maximale Länge für den NTP Server Namen sind 20 Stellen
    for (int i = 0; i < 20; i++)
    {
        if (myntp_name[i] == 0)
        {
            // Letzte Stelle wird NULL gesetzt
            EEPROM.put(NETWORK_CONFIG_START + 16 + i * sizeof(char), 0);
            break;
        }
        else
        {
            EEPROM.put(NETWORK_CONFIG_START + 16 + i * sizeof(char), myntp_adr[i]);
        }
    }
}

void Storage::read_network_time_settings(char *myntp_name, uint8_t *myntp_adr)
{
    // 32 bit Gateway Address
    myntp_adr[0] = EEPROM.read(NETWORK_CONFIG_START + 12);
    myntp_adr[1] = EEPROM.read(NETWORK_CONFIG_START + 13);
    myntp_adr[2] = EEPROM.read(NETWORK_CONFIG_START + 14);
    myntp_adr[3] = EEPROM.read(NETWORK_CONFIG_START + 15);

    // Maximale Länge für den NTP Server Namen sind 20 Stellen
    for (int i = 0; i < 20; i++)
    {
        char read = EEPROM.read(NETWORK_CONFIG_START + 16 + i * sizeof(char));
        if (read == '\0')
        {
            // Letzte Stelle wird NULL gesetzt
            myntp_name[i] = '\0';
            break;
        }
        else
        {
            myntp_name[i] = read;
        }
    }
}
