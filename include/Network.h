#include "DEFINITIONS.h"
#ifdef DEBUG_NETWORK
#define DEBUG
#endif

#include <Arduino.h>
#include <EtherCard.h>

namespace Network
{
    extern int8_t NetworkStatus;
    const char hostname[] PROGMEM = HOSTNAME;
    const char timeServer[] PROGMEM = "pool.ntp.org";
    const byte mac_address[6] PROGMEM = MAC_ADDRESS;
    const unsigned int NTP_LOCALPORT PROGMEM = 8888;
    const unsigned int NTP_PACKET_SIZE PROGMEM = 48;
    const unsigned int NTP_REMOTEPORT PROGMEM = 123;

    // Network Configuration
    bool init_ethernet();
    bool is_cable_connected();
    bool static_setup(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t snm);
    bool dhcp_setup();
    bool using_dhcp();
    uint8_t *get_ip();
    uint8_t *get_gw();
    uint8_t *get_dns();
    uint8_t *get_snm();
    uint8_t get_prefix();

    bool http_response(byte day, byte month, byte year, byte hour, byte minute, byte second);
}