#define DEBUG

#include <Arduino.h>
#include <EtherCard.h>
#include "Network.h"

#define CHIP_SELECT 14

byte Ethernet::buffer[700];

class Network
{
private:
    bool dhcp;
    byte *ip_address;
    byte *mac_address;
    byte *gw_address;
    byte *subnetmask;
    byte *dns_address;
    bool connected;

public:
    Network()
    {
        this->dhcp = true;
        this->connected = false;
    }

    Network(byte ip_address[4], byte gw_address[4], byte dns_address[4])
    {
        this->ip_address = ip_address;
        this->gw_address = gw_address;
        this->dns_address = gw_address;
        this->subnetmask = subnetmask;
        this->connected = false;
        this->dhcp = false;
    }

#pragma region setter getter methods
    void set_ip(byte ip_address[4])
    {
        this->ip_address = ip_address;
    }
    byte *get_ip()
    {
        return this->ip_address;
    }

    void set_gw(byte gw_address[4])
    {
        this->gw_address = gw_address;
    }

    byte *get_gw()
    {
        return this->ip_address;
    }

    void set_dns(byte dns_address[4])
    {
        this->dns_address = dns_address;
    }

    byte *get_dns()
    {
        return this->dns_address;
    }
#pragma endregion

    bool init_network()
    {
#ifdef DEBUG
        Serial.println("[Info] Ethernet wird aktiviert ...");
#endif
        if (ether.begin(sizeof Ethernet::buffer, this->mac_address, CHIP_SELECT) == 0)
        {
            this->connected = false;
            return false;
#ifdef DEBUG
            Serial.println("[Error] Ethernet konnte nicht aktiviert werden.");
#endif
        }
#ifdef DEBUG
        Serial.println("[Info] Ethernet wurde aktiviert.");
#endif

        if (dhcp)
        {
#ifdef DEBUG
            Serial.println(F("[Info] Setup DHCP"));
#endif
            if (!ether.dhcpSetup())
            {
#ifdef DEBUG
                Serial.println("[Error] DHCP ist fehlgeschlagen");
#endif
                return -1;
            }
        }
        else
        {
#ifdef DEBUG
            Serial.println(F("[Info] Setup Static-IP"));
#endif
            ether.staticSetup(this->ip_address, this->gw_address, this->dns_address, this->subnetmask);
        }
#ifdef DEBUG
        ether.printIp("[Info] IP-Adressse   : ", ether.myip);
        ether.printIp("[Info] Netmask       : ", ether.netmask);
        ether.printIp("[Info] Gateway       : ", ether.gwip);
        ether.printIp("[Info] DNS           : ", ether.dnsip);
#endif
    }
};