#include "DEFINITIONS.h"
#ifdef DEBUG_NETWORK
#define DEBUG
#endif

#include <Arduino.h>
#include <EtherCard.h>

namespace Network
{
    // Struct for network settings and activities
    struct NetworkStatus
    {
        bool active;
        bool linkup;
        bool DhcpEnabled;
        bool NtpEnabled;
        bool HttpEnabled;
        uint8_t ip[4], gw[4], dns[4];
        uint8_t snm;
        uint8_t error_code;
    };

    // Defined in Network.cpp
    extern NetworkStatus nw_status;

    // Read-Only settings 
    const char hostname[] PROGMEM = HOSTNAME;
    const static uint8_t mac_address[6] = MAC_ADDRESS;

    // Network Configuration
    void init_ethernet();
    void check_link();
    bool static_setup(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t snm);
    void dhcp_setup();
    uint8_t *get_ip();
    uint8_t *get_gw();
    uint8_t *get_dns();
    uint8_t *get_snm();
    uint8_t get_prefix();

    bool http_response(byte day, byte month, byte year, byte hour, byte minute, byte second);
}