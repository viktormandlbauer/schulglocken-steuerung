// Angabe des Header Files
#include "Storage.h"

#define STORAGE_ALARMS_START 0
#define STORAGE_ALARMS_ASSIGNMENT_START MAXIMUM_AMOUNT_ALARMS * 2 + 1
#define STORAGE_ALARM_TYPES_START STORAGE_ALARMS_ASSIGNMENT_START + MAXIMUM_AMOUNT_ALARMS * 1 + 1
#define STORAGE_NETWORK_CONFIG_START STORAGE_ALARM_TYPES_START + MAXIMUM_AMOUNT_ALARM_TYPES * 8 + 1
#define STORAGE_NETWORK_DHCP_ENABLED STORAGE_NETWORK_CONFIG_START + 36 + 1  + 300
#define STORAGE_NETWORK_NTP_ENABlED STORAGE_NETWORK_DHCP_ENABLED + 1
#define NEXT STORAGE_NETWORK_NTP_ENABlED + 1

namespace Storage
{

    void save_alarm_count(uint8_t alarm_count)
    {
        EEPROM.put(STORAGE_ALARMS_START, alarm_count);
#ifdef DEBUG
        Serial.print(F("[Info] (Storage) Saved alarm count: "));
        Serial.println(alarm_count);
#endif
    }

    void save_alarm(uint16_t alarm, uint8_t alarms_type_assignment, uint8_t index)
    {
        EEPROM.put(1 + STORAGE_ALARMS_START + sizeof(uint16_t) * index, alarm);
        EEPROM.put(STORAGE_ALARMS_ASSIGNMENT_START + sizeof(uint8_t) * index, alarms_type_assignment);

#ifdef DEBUG
        Serial.print(F("[Info] (Storage) Saved alarm: "));
        Serial.println(alarm);
        Serial.print(F("[Info] (Storage) Saved alarm type assignment: "));
        Serial.println(alarms_type_assignment);
#endif
    }

    uint8_t read_alarm_count()
    {
#ifdef DEBUG
        uint8_t alarm_count = EEPROM.read(STORAGE_ALARMS_START);
        Serial.print(F("[Info] (Storage) Read alarm count : "));
        Serial.println(alarm_count);
        return alarm_count;
#endif
        return EEPROM.read(STORAGE_ALARMS_START);
    }

    void read_alarm(uint16_t *alarm, uint8_t *alarms_type_assignment, uint8_t index)
    {
        EEPROM.get(1 + STORAGE_ALARMS_START + sizeof(uint16_t) * index, *alarm);
        EEPROM.get(STORAGE_ALARMS_ASSIGNMENT_START + sizeof(uint8_t) * index, *alarms_type_assignment);
#ifdef DEBUG
        Serial.print(F("[Info] (Storage) Read alarm: "));
        Serial.print(*alarm);
        Serial.print(F(" with type: "));
        Serial.print(*alarms_type_assignment);
        Serial.print(F(" at index: "));
        Serial.println(index);
#endif
    }

    void save_alarm_types(uint64_t *alarm_types, uint8_t count)
    {
        /**
         * Speichern von Alarmtypen (64bit Werte)
         */

        // Anzahl wird an erster Stelle des Addressbereichs gespeichert.
        EEPROM.put(STORAGE_ALARM_TYPES_START, count);

        for (int i = 0; i < count; i++)
        {
            EEPROM.put(1 + STORAGE_ALARM_TYPES_START + sizeof(uint64_t) * i, alarm_types[i]);

#ifdef DEBUG
            Serial.print(F("[Info] (Storage) Alarmtyp wurde gespeichert."));
#endif
        }
    }

    uint8_t read_alarm_types(uint64_t *alarm_types)
    {
        /**
         * Auslesen von Alarmtypen
         */

        uint8_t count = EEPROM.read(STORAGE_ALARM_TYPES_START);

        for (int i = 0; i < count; i++)
        {
            EEPROM.get(1 + STORAGE_ALARM_TYPES_START + sizeof(uint64_t), alarm_types[i]);
#ifdef DEBUG
            Serial.print(F("[Info] (Storage) Alarmtyp wurde ausgelesen."));
#endif
        }

        return count;
    }

    void save_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t prefix)
    {
        // 32 bit IP Address
        EEPROM.put(STORAGE_NETWORK_CONFIG_START, ip[0]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 1, ip[1]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 2, ip[2]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 3, ip[3]);

        // 32 bit Gateway Address
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 4, gw[0]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 5, gw[1]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 6, gw[2]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 7, gw[3]);

        // 32 bit DNS Server Address
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 8, dns[0]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 9, dns[1]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 10, dns[2]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 11, dns[3]);

        // 0 - 32 Subnetmask Suffix
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 12, prefix);

#ifdef DEBUG

        char buffer[16];
        sprintf(buffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        Serial.print(F("[Info] (Storage) Saved IP: "));
        Serial.println(buffer);

        sprintf(buffer, "%d.%d.%d.%d", gw[0], gw[1], gw[2], gw[3]);
        Serial.print(F("[Info] (Storage) Saved Gateway: "));
        Serial.println(buffer);

        sprintf(buffer, "%d.%d.%d.%d", dns[0], dns[1], dns[2], dns[3]);
        Serial.print(F("[Info] (Storage) Saved DNS: "));
        Serial.println(buffer);

        Serial.print(F("[Info] (Storage) Saved Prefix: "));
        Serial.println(prefix);

#endif
    }
    void read_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t *prefix)
    {
        // 32 bit IP Address
        ip[0] = EEPROM.read(STORAGE_NETWORK_CONFIG_START);
        ip[1] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 1);
        ip[2] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 2);
        ip[3] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 3);

        // 32 bit Gateway Address
        gw[0] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 4);
        gw[1] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 5);
        gw[2] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 6);
        gw[3] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 7);

        // 32 bit DNS Server Address
        dns[0] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 8);
        dns[1] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 9);
        dns[2] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 10);
        dns[3] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 11);

        // Read 1 byte SNM Suffix
        *prefix = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 12);

#ifdef DEBUG
        char buffer[16];
        sprintf(buffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        Serial.print(F("[Info] (Storage) Read IP: "));
        Serial.println(buffer);

        sprintf(buffer, "%d.%d.%d.%d", gw[0], gw[1], gw[2], gw[3]);
        Serial.print(F("[Info] (Storage) Read Gateway: "));
        Serial.println(buffer);

        sprintf(buffer, "%d.%d.%d.%d", dns[0], dns[1], dns[2], dns[3]);
        Serial.print(F("[Info] (Storage) Read DNS: "));
        Serial.println(buffer);

        Serial.print(F("[Info] (Storage) Read Prefix: "));
        Serial.println(*prefix);
#endif
    }

    void save_network_time_settings(char *myntp_name, uint8_t *myntp_adr)
    {

        // 32 bit NTP Server IP Address
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 13, myntp_adr[0]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 14, myntp_adr[1]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 15, myntp_adr[2]);
        EEPROM.put(STORAGE_NETWORK_CONFIG_START + 16, myntp_adr[3]);

        // Maximale Länge für den NTP Server Namen sind 20 Stellen
        for (int i = 0; i < 20; i++)
        {
            if (myntp_name[i] == 0)
            {
                // Letzte Stelle wird NULL gesetzt
                EEPROM.put(STORAGE_NETWORK_CONFIG_START + 17 + i * sizeof(char), 0);
                break;
            }
            else
            {
                EEPROM.put(STORAGE_NETWORK_CONFIG_START + 17 + i * sizeof(char), myntp_adr[i]);
            }
        }
    }

    void read_network_time_settings(char *myntp_name, uint8_t *myntp_adr)
    {
        // 32 bit Gateway Address
        myntp_adr[0] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 13);
        myntp_adr[1] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 14);
        myntp_adr[2] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 15);
        myntp_adr[3] = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 16);

        // Maximale Länge für den NTP Server Namen sind 20 Stellen
        for (int i = 0; i < 20; i++)
        {
            char read = EEPROM.read(STORAGE_NETWORK_CONFIG_START + 17 + i * sizeof(char));
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

    void save_network_dhcp(bool automatic)
    {
#ifdef DEBUG
        Serial.print(F("[Info] (Storage) Save DHCP enabled: "));
        Serial.println(automatic, BIN);
#endif

        EEPROM.put(STORAGE_NETWORK_DHCP_ENABLED, automatic);
    }

    bool read_network_dhcp()
    {
#ifdef DEBUG
        bool automatic = EEPROM.read(STORAGE_NETWORK_DHCP_ENABLED);
        Serial.print(F("[Info] (Storage) Read DHCP enabled: "));
        Serial.println(automatic, BIN);
        return automatic;
#endif
        return EEPROM.read(STORAGE_NETWORK_DHCP_ENABLED);
    }

    void save_network_ntp(bool isEnabled)
    {
#ifdef DEBUG
        Serial.print(F("[Info] (Storage) Save NTP enabled: "));
        Serial.println(isEnabled, BIN);
#endif
        EEPROM.put(STORAGE_NETWORK_NTP_ENABlED, isEnabled);
    }

    bool read_network_ntp()
    {
#ifdef DEBUG
        bool isEnabled = EEPROM.read(STORAGE_NETWORK_NTP_ENABlED);
        Serial.print(F("[Info] (Storage) Read NTP enabled: "));
        Serial.println(isEnabled, BIN);
        return isEnabled;
#endif
        return (bool)EEPROM.read(STORAGE_NETWORK_NTP_ENABlED);
    }
}