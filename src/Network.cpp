#include "Network.h"

byte Ethernet::buffer[700];

namespace Network
{
    int8_t NetworkStatus;

    bool init_ethernet()
    {
#ifdef DEBUG
        Serial.println("[Info] (Network) Activate ethernet ...");
#endif
        if (ether.begin(sizeof Ethernet::buffer, mac_address, CHIP_SELECT) == 0)
        {
#ifdef DEBUG
            Serial.println("[Error] (Network) Failed to active ethernet.");
#endif
            NetworkStatus = ETHERNET_INITIALIZED_FAILED;
            return false;
        }
#ifdef DEBUG
        Serial.println("[Info] (Network) Ethernet activated.");
#endif
        NetworkStatus = ETHERNET_INITIALIZED;
        return true;
    }

    bool is_cable_connected()
    {
        uint8_t count = 0;
        while (!ether.isLinkUp())
        {
            count += 1;
            if (count > 254)
            {
                NetworkStatus = ETHERNET_LINKDOWN;
                return false;
            }
            delay(1);
        }
        NetworkStatus = ETHERNET_LINKUP;
        return true;
    }

    bool dhcp_setup()
    {
#ifdef DEBUG
        Serial.println("[Info] (Network) Starting DHCP setup.");
#endif
        if (ether.dhcpSetup(hostname))
        {
#ifdef DEBUG
            Serial.println("[Info] (Network) DHCP setup successful!");
            ether.printIp("[Info] IP:   ", ether.myip);
            ether.printIp("[Info] GW:   ", ether.gwip);
            ether.printIp("[Info] DNS:  ", ether.dnsip);
            ether.printIp("[Info] NM:   ", ether.netmask);
#endif
            NetworkStatus = ETHERNET_DHCP_SUCCESS;
            return true;
        }
        else
        {
#ifdef DEBUG
            Serial.println("[Error] (Network) DHCP failed.");
#endif
            NetworkStatus = ETHERNET_DHCP_FAILED;
            return false;
        }
    }

    bool static_setup(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix)
    {
        uint8_t netmask[4];

        // Konvertierung von Prefix Notation in Subnetzmaske
        unsigned long mask = (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;
        netmask[0] = mask >> 24;
        netmask[1] = (mask >> 16) & 0xFF;
        netmask[2] = (mask >> 8) & 0xFF;
        netmask[3] = mask & 0xFF;

        if (ether.staticSetup(ip, gw, dns, netmask))
        {
#ifdef DEBUG
            Serial.println(F("[Info] (Network) Static network successful!"));
            ether.printIp("[Info] (Network) IP:   ", ether.myip);
            ether.printIp("[Info] (Network) GW:   ", ether.gwip);
            ether.printIp("[Info] (Network) DNS:  ", ether.dnsip);
            ether.printIp("[Info] (Network) NM:   ", ether.netmask);
#endif
            NetworkStatus = ETHERNET_STATIC_SUCCESS;
            return true;
        }
        else
        {
#ifdef DEBUG
            Serial.println("[Error] (Network) Static network setup failed!");
#endif
            NetworkStatus = ETHERNET_STATIC_FAILED;
            return false;
        }
    }

    BufferFiller bfill;
    bool http_response(byte day, byte month, byte year, byte hour, byte minute, byte second)
    {
        word len = ether.packetReceive();
        word pos = ether.packetLoop(len);

        if (pos)
        {
            bfill = ether.tcpOffset();
            bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n<meta http-equiv='refresh' content='1'/><title>Schulglockensteuerung</title><h1>$D$D:$D$D:$D$D</h1>"),
                         hour / 10, hour % 10, minute / 10, minute % 10, second / 10, second % 10);
            ether.httpServerReply(bfill.position());
        }
    }

    uint8_t *get_ip()
    {
        return ether.myip;
    }

    uint8_t *get_gw()
    {
        return ether.gwip;
    }

    uint8_t *get_dns()
    {
        return ether.dnsip;
    }

    uint8_t *get_snm()
    {
        return ether.netmask;
    }

    uint8_t get_prefix()
    {
        uint8_t *snm = ether.netmask;
        uint8_t snm_prefix = 0;
        for (uint8_t i = 0; i < 4; i++)
        {
            for (uint8_t j = 0; j < 8; j++)
            {
                if (snm[i] & (0b1 << j))
                {
                    snm_prefix += 1;
                }
            }
        }
        return snm_prefix;
    }

}
