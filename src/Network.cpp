#include "DEBUG.h"
#ifdef DEBUG_NETWORK
#define DEBUG
#endif

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
    this->set_ip(ip_address);
    this->set_gw(gw_address);
    this->set_dns(dns_address);
    this->set_subnetmask(subnetmask);
    connected = false;
    dhcp = false;
}

void Network::set_ip(byte ip_address[4])
{
    this->ip_address = ip_address;
#ifdef DEBUG
    ether.printIp("[Info] Set IP-Addr.: ", ether.myip);
#endif
}
byte *Network::get_ip()
{
    return this->ip_address;
}

void Network::set_gw(byte gw_address[4])
{
#ifdef DEBUG
    ether.printIp("[Info] Set gatway:   ", ether.gwip);
#endif
    this->gw_address = gw_address;
}

byte *Network::get_gw()
{
    return this->ip_address;
}

void Network::set_dns(byte dns_address[4])
{
#ifdef DEBUG
    ether.printIp("[Info] Set DNS:      ", ether.dnsip);
#endif
    this->dns_address = dns_address;
}

byte *Network::get_dns()
{
    return this->dns_address;
}

void Network::set_subnetmask(byte subnetmask[4])
{
#ifdef DEBUG
    ether.printIp("[Info] Set SNM:      ", ether.netmask);
#endif
    this->subnetmask = subnetmask;
}

byte *Network::get_subnetmask()
{
    return this->subnetmask;
}

bool Network::init_network()
{
#ifdef DEBUG
    Serial.println("[Info] Activate ethernet ...");
#endif
    if (ether.begin(sizeof Ethernet::buffer, this->mac_address, CHIP_SELECT) == 0)
    {
        this->connected = false;
        return false;
#ifdef DEBUG
        Serial.println("[Error] Failed to active ethernet.");
#endif
    }
#ifdef DEBUG
    Serial.println("[Info] Ethernet activated.");
#endif

    if (dhcp)
    {
#ifdef DEBUG
        Serial.println(F("[Info] Setup DHCP"));
#endif
        if (!ether.dhcpSetup())
        {
#ifdef DEBUG
            Serial.println("[Error] DHCP failed.");
#endif
            return false;
        }
        else
        {
            this->set_ip(ether.myip);
            this->set_dns(ether.dnsip);
            this->set_gw(ether.gwip);
            this->set_subnetmask(ether.netmask);
#ifdef DEBUG
            Serial.println("[Info] DHCP successfull");
#endif
        }
    }
    else
    {
#ifdef DEBUG
        Serial.println(F("[Info] Setup Static-IP"));
#endif
        ether.staticSetup(this->ip_address, this->gw_address, this->dns_address, this->subnetmask);
    }
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
