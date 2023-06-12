#include "Network.h"
byte Ethernet::buffer[700];

namespace Network
{
    NetworkStatus nw_status;

    bool init_ethernet()
    {
#ifdef DEBUG
        Serial.println(F("[Info] (Network) Activate ethernet ..."));
#endif
        if (ether.begin(sizeof Ethernet::buffer, mac_address, CS_ETHERNET) == 0)
        {
#ifdef DEBUG
            Serial.println(F("[Error] (Network) Failed to active ethernet."));
#endif
            nw_status.active = false;
            return false;
        }
#ifdef DEBUG
        Serial.println(F("[Info] (Network) Ethernet activated."));
#endif
        nw_status.active = true;
        return true;
    }

    bool check_link()
    {
        uint8_t count = 0;
        while (!ether.isLinkUp())
        {
            count += 1;
            if (count > 254)
            {
#ifdef DEBUG
                Serial.println(F("[Info] (Network) No cable connected"));
#endif
                nw_status.linkup = false;
                return false;
            }
            delayMicroseconds(1);
        }
#ifdef DEBUG
        Serial.println(F("[Info] (Network) Cable connected"));
#endif
        nw_status.linkup = true;
        return true;
    }

    bool dhcp_setup()
    {
#ifdef DEBUG
        Serial.println(F("[Info] (Network) Starting DHCP setup."));
#endif
        if (!ether.dhcpSetup(hostname))
        {
#ifdef DEBUG
            Serial.println(F("[Error] (Network) DHCP failed."));
#endif
            nw_status.error_code = ETHERNET_DHCP_FAILED;
            return false;
        }
#ifdef DEBUG

        Serial.println(F("[Info] (Network) DHCP setup successful!"));
        ether.printIp(F("[Info] IP:   "), ether.myip);
        ether.printIp(F("[Info] GW:   "), ether.gwip);
        ether.printIp(F("[Info] DNS:  "), ether.dnsip);
        ether.printIp(F("[Info] NM:   "), ether.netmask);
#endif
        nw_status.error_code = 0;
        return true;
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

        if (!ether.staticSetup(ip, gw, dns, netmask))
        {
#ifdef DEBUG
            Serial.println(F("[Error] (Network) Static network setup failed!"));
#endif
            nw_status.error_code = ETHERNET_STATIC_FAILED;
            return false;
        }

#ifdef DEBUG
        Serial.println(F("[Info] (Network) Static network successful!"));
        ether.printIp(F("[Info] (Network) IP:   "), ether.myip);
        ether.printIp(F("[Info] (Network) GW:   "), ether.gwip);
        ether.printIp(F("[Info] (Network) DNS:  "), ether.dnsip);
        ether.printIp(F("[Info] (Network) NM:   "), ether.netmask);
#endif

        nw_status.error_code = 0;
        return true;
    }

    BufferFiller bfill;
    void http_response(char *buffer)
    {
        word len = ether.packetReceive();
        word pos = ether.packetLoop(len);
        if (pos)
        {
#ifdef DEBUG
            Serial.println(F("[Info] (Network) HTTP Response"));
#endif
            bfill = ether.tcpOffset();
            bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n<title>Schulglockensteuerung</title><h1>$S</h1>"),
                         buffer);
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
