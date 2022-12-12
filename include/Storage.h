#include <EEPROM.h>

// Arduino Atmega 4096 bytes
/**
 * Überlegung Speicherbelegung EEPROM
 *
 * Tagesläutzeiten 0-1439 || Läutdauer
 * 16 bit Array
 * 16 * 100 = 1600
 * 200 byte
 *
 * Netzwerkaddressen:
 *  ip, gw, dns-> 3 x 4byte
 *  ntp -> 12 byte + 4 byte (text + address)
 *
 * Ausnahmen
 * 48 byte
 *
 */

// Umsetzung Adressbereiche EEPROM
/**
 * @TODO
 * Byte 0 - 199: Tagesläutzeiten
 *
 *
 */

namespace Storage
{
    byte read_stored_alarms(uint16_t *alarms);
    void save_alarms(uint16_t *alarms, byte size);

    void save_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);
    byte read_network_settings(uint8_t *myip, uint8_t *mygw, uint8_t *mydns);

    void save_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
    byte read_network_time_settings(char *myntp_name, uint8_t *myntp_adr);
}