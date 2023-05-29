#include "DEFINITIONS.h"
#ifdef DEBUG_NETWORK
#define DEBUG
#endif

#include <Arduino.h>
#include <EtherCard.h>

namespace Network
{
    // Struktur für die Netzwerkeinstellung
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

    // Definition für externe Abfragen der Netzwerkeinstellung
    extern NetworkStatus nw_status;

    // Hostname für DHCP Konfiguration
    const char hostname[] PROGMEM = HOSTNAME;

    // MAC Adresse 
    const static uint8_t mac_address[6] = MAC_ADDRESS;

    // Initialisierung der Ethernet Schnittstelle
    bool init_ethernet();

    // Test ob Kabel angeschlossen ist
    bool check_link();

    // Setup für statische IP Konfiguration 
    bool static_setup(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t snm);
    
    // Setup für DHCP
    bool dhcp_setup();

    // HTTP Server ausgabe
    void http_response(char buffer[20]);

    // Rückgabe der IP Adresse
    uint8_t *get_ip();

    // Rückgabe der Gateway Adresse
    uint8_t *get_gw();

    // Rückgabe der DNS Adresse
    uint8_t *get_dns();

    // Rückgabe der Subntzmaske
    uint8_t *get_snm();

    //Rückgabe des Präfixes der Subnetzmaske
    uint8_t get_prefix();

}