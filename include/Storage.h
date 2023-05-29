#include "DEFINITIONS.h"
#ifdef DEBUG_STORAGE
#define DEBUG
#endif

#include <Arduino.h>
#include <EEPROM.h>

namespace Storage
{
    // Liest die Anzahl der Alarme aus dem EEPROM
    uint8_t read_alarm_count();

    // Liest Alarm am gegebenen Index aus dem EEPROM
    void read_alarm(uint16_t *alarms, uint8_t *alarms_type_assignment, uint8_t index);

    // Schreibt die Anzahl der Alarme in den EEPROM
    void save_alarm_count(uint8_t alarm_count);

    // Schreibt den Alarm an gegebenen Index in den EEPROM
    void save_alarm(uint16_t alarm, uint8_t alarms_type_assignment, uint8_t index);

    // Liest die Alarmtypen aus dem EEPROM
    uint8_t read_alarm_types(uint64_t *alarm_types);
    
    // Schreibt die Alarmtypen in den EEPROM
    void save_alarm_types(uint64_t *alarm_types, uint8_t count);
    
    // Schreibt die statische IP Konfiguration in den EEPROM
    void save_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t prefix);

    // Liest die statische IP Konfiguration aus dem EEPROM
    void read_network_settings(uint8_t *ip, uint8_t *gw, uint8_t *dns, uint8_t *prefix);

    // Schreitb die DHCP Einstellung in den EEPROM
    void save_network_dhcp(bool automatic);

    // Liest die DHCP aus dem EEPROM 
    bool read_network_dhcp();

    // Schreibt die NTP Einstellung in dem EEPROM
    void save_network_ntp(bool isEnabled);

    // Liest die NTP Einstellung aus dem EEPROM (true -> NTP enabled)
    bool read_network_ntp();

    // Liest die Anzahl der Alarmausnahmen aus dem EEPROM aus
    uint8_t read_exception_count();

    // Schreibt die Anzahl der Alarmausnahmen aus dem EEPROM
    void save_exception_count(uint8_t exception_count);
    
    // Schreibt die Alarmausnahme an gegebenen Index in den EEPROM
    void save_exception(uint8_t BeginDay, uint8_t BeginMonth, uint8_t EndDay, uint8_t EndMonth, bool reoccurring, uint8_t index);

    // Liest die Alarmausnahme an gegebenen Index aus dem EEPROM aus
    void read_exception(uint8_t *BeginDay, uint8_t *BeginMonth, uint8_t *EndDay, uint8_t *EndMonth, bool *reoccurring, uint8_t index);
    
    // Schreibt die Wochentagsausnahmen in den EEPROM 
    void save_weekday_exceptions(uint8_t weekday_exception_list);
    
    // Liest die Wochentagsausnahmen aus dem EEPROM aus
    uint8_t read_weekday_exceptions();
}