#define DEBUG

#include "Network.h"

#define CHIP_SELECT 14

byte Ethernet::buffer[700];

Network::Network()
{
    dhcp = true;
    connected = false;
}

Network::Network(byte ip_address[4], byte gw_address[4], byte dns_address[4], byte subnetmask[4])
{
    ip_address = ip_address;
    gw_address = gw_address;
    dns_address = gw_address;
    subnetmask = subnetmask;
    connected = false;
    dhcp = false;
}

void Network::set_ip(byte ip_address[4])
{
    this->ip_address = ip_address;
}
byte *Network::get_ip()
{
    return this->ip_address;
}

void Network::set_gw(byte gw_address[4])
{
    this->gw_address = gw_address;
}

byte *Network::get_gw()
{
    return this->ip_address;
}

void Network::set_dns(byte dns_address[4])
{
    this->dns_address = dns_address;
}

byte *Network::get_dns()
{
    return this->dns_address;
}

bool Network::init_network()
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
            return false;
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
    return true;
}

bool Network::test_gateway()
{
    while (ether.clientWaitingGw())
    {
    }
    ether.packetLoop(ether.packetReceive());
    Serial.println("Gateway found");

    return true;
}

bool Network::test_ntp()
{

    return true;
}
