#include "DEBUG.h"
#ifdef DEBUG_NETWORK
#define DEBUG
#endif

#include "Network.h"
#include "HTTP_PAGE.h"

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

void Network::set_ntpsrv(String server)
{
#ifdef DEBUG
    Serial.println("[Info] Set NTP-Server:  ");
    Serial.print(server);
#endif
}

String Network::get_ntpsrv()
{
    return this->ntpsrv;
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

bool Network::check_network()
{
}

bool Network::test_gateway()
{
    while (ether.clientWaitingGw())
        ;
    ether.packetLoop(ether.packetReceive());
#ifdef DEBUG
    Serial.println("Gateway found");
#endif
    return true;
}

void sendNTPpacket(const uint8_t *remoteAddress)
{
    // buffer to hold outgoing packet
    byte packetBuffer[NTP_PACKET_SIZE];
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    ether.sendUdp(packetBuffer, NTP_PACKET_SIZE, NTP_LOCALPORT, remoteAddress, NTP_REMOTEPORT);
    Serial.println("NTP request sent.");
}

void udpReceiveNtpPacket(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *packetBuffer, uint16_t len)
{
    Serial.println("NTP response received.");

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;

    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;

    // print Unix time:
    Serial.print("Unix time = ");
    Serial.println(epoch);
}

unsigned long Network::get_ntp_time()
{
    bool dns_reachable;
    if (!ether.dnsLookup(this->ntp_address))
    {
        dns_reachable = false;
    }
    else
    {
        dns_reachable = true;
        return false;
    }

    uint8_t ntpIp[IP_LEN];
    ether.copyIp(ntpIp, ether.hisip);
    ether.printIp("NTP: ", ntpIp);

#ifdef DEBUG
    if (dns_reachable)
    {
        Serial.println("DNS failed");
    }
#endif

    ether.udpServerListenOnPort(&udpReceiveNtpPacket, NTP_LOCALPORT);
    Serial.println("Started listening for response.");
    ether.packetLoop(ether.packetReceive());

    sendNTPpacket(ntpIp);

    // TODO:
    // Verbindungsaufbau mit NTP Server
    return true;
}

bool Network::show_http_status()
{
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply((sizeof page) - 1);
}