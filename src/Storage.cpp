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

void Storage::save_alarms(uint16_t alarms[], uint8_t alarms_type_assignment[], uint8_t count)
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

uint8_t Storage::read_alarms(uint16_t alarms[], uint8_t alarms_type_assignment[])
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

void Storage::save_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t prefix)
{
    // 32 bit IP Address
    EEPROM.put(NETWORK_CONFIG_START, ip[0]);
    EEPROM.put(NETWORK_CONFIG_START + 1, ip[1]);
    EEPROM.put(NETWORK_CONFIG_START + 2, ip[2]);
    EEPROM.put(NETWORK_CONFIG_START + 3, ip[3]);

    // 32 bit Gateway Address
    EEPROM.put(NETWORK_CONFIG_START + 4, gw[0]);
    EEPROM.put(NETWORK_CONFIG_START + 5, gw[1]);
    EEPROM.put(NETWORK_CONFIG_START + 6, gw[2]);
    EEPROM.put(NETWORK_CONFIG_START + 7, gw[3]);

    // 32 bit DNS Server Address
    EEPROM.put(NETWORK_CONFIG_START + 8, dns[0]);
    EEPROM.put(NETWORK_CONFIG_START + 9, dns[1]);
    EEPROM.put(NETWORK_CONFIG_START + 10, dns[2]);
    EEPROM.put(NETWORK_CONFIG_START + 11, dns[3]);

    // 0 - 32 Subnetmask Suffix
    EEPROM.put(NETWORK_CONFIG_START + 12, prefix);

#ifdef DEBUG

    char buffer[16];
    sprintf(buffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    Serial.print("[Info] (Storage) Saved IP: ");
    Serial.println(buffer);

    sprintf(buffer, "%d.%d.%d.%d", gw[0], gw[1], gw[2], gw[3]);
    Serial.print("[Info] (Storage) Saved Gateway: ");
    Serial.println(buffer);

    sprintf(buffer, "%d.%d.%d.%d", dns[0], dns[1], dns[2], dns[3]);
    Serial.print("[Info] (Storage) Saved DNS: ");
    Serial.println(buffer);

    Serial.print("[Info] (Storage) Saved Prefix: ");
    Serial.println(prefix);

#endif
}
void Storage::read_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t *prefix)
{
    // 32 bit IP Address
    ip[0] = EEPROM.read(NETWORK_CONFIG_START);
    ip[1] = EEPROM.read(NETWORK_CONFIG_START + 1);
    ip[2] = EEPROM.read(NETWORK_CONFIG_START + 2);
    ip[3] = EEPROM.read(NETWORK_CONFIG_START + 3);

    // 32 bit Gateway Address
    gw[0] = EEPROM.read(NETWORK_CONFIG_START + 4);
    gw[1] = EEPROM.read(NETWORK_CONFIG_START + 5);
    gw[2] = EEPROM.read(NETWORK_CONFIG_START + 6);
    gw[3] = EEPROM.read(NETWORK_CONFIG_START + 7);

    // 32 bit DNS Server Address
    dns[0] = EEPROM.read(NETWORK_CONFIG_START + 8);
    dns[1] = EEPROM.read(NETWORK_CONFIG_START + 9);
    dns[2] = EEPROM.read(NETWORK_CONFIG_START + 10);
    dns[3] = EEPROM.read(NETWORK_CONFIG_START + 11);

    // Read 1 byte SNM Suffix
    *prefix = EEPROM.read(NETWORK_CONFIG_START + 12);

#ifdef DEBUG

    char buffer[16];
    sprintf(buffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    Serial.print("[Info] (Storage) Read IP: ");
    Serial.println(buffer);

    sprintf(buffer, "%d.%d.%d.%d", gw[0], gw[1], gw[2], gw[3]);
    Serial.print("[Info] (Storage) Read Gateway: ");
    Serial.println(buffer);

    sprintf(buffer, "%d.%d.%d.%d", dns[0], dns[1], dns[2], dns[3]);
    Serial.print("[Info] (Storage) Read DNS: ");
    Serial.println(buffer);

    Serial.print("[Info] (Storage) Read Prefix: ");
    Serial.println(*prefix);

#endif
}

void Storage::save_network_time_settings(char *myntp_name, uint8_t *myntp_adr)
{

    // 32 bit NTP Server IP Address
    EEPROM.put(NETWORK_CONFIG_START + 13, myntp_adr[0]);
    EEPROM.put(NETWORK_CONFIG_START + 14, myntp_adr[1]);
    EEPROM.put(NETWORK_CONFIG_START + 15, myntp_adr[2]);
    EEPROM.put(NETWORK_CONFIG_START + 16, myntp_adr[3]);

    // Maximale Länge für den NTP Server Namen sind 20 Stellen
    for (int i = 0; i < 20; i++)
    {
        if (myntp_name[i] == 0)
        {
            // Letzte Stelle wird NULL gesetzt
            EEPROM.put(NETWORK_CONFIG_START + 17 + i * sizeof(char), 0);
            break;
        }
        else
        {
            EEPROM.put(NETWORK_CONFIG_START + 17 + i * sizeof(char), myntp_adr[i]);
        }
    }
}

void Storage::read_network_time_settings(char *myntp_name, uint8_t *myntp_adr)
{
    // 32 bit Gateway Address
    myntp_adr[0] = EEPROM.read(NETWORK_CONFIG_START + 13);
    myntp_adr[1] = EEPROM.read(NETWORK_CONFIG_START + 14);
    myntp_adr[2] = EEPROM.read(NETWORK_CONFIG_START + 15);
    myntp_adr[3] = EEPROM.read(NETWORK_CONFIG_START + 16);

    // Maximale Länge für den NTP Server Namen sind 20 Stellen
    for (int i = 0; i < 20; i++)
    {
        char read = EEPROM.read(NETWORK_CONFIG_START + 17 + i * sizeof(char));
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
