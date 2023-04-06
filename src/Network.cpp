#include "Network.h"


byte Ethernet::buffer[700];
using namespace Network;
int8_t network_status;

bool Network::init_ethernet()
{
#ifdef DEBUG
    Serial.println("[Info] (Network) Activate ethernet ...");
#endif

    uint8_t vers = ether.begin(sizeof Ethernet::buffer, mac_address, CHIP_SELECT);
    if (vers == 0)
    {
#ifdef DEBUG
        Serial.println("[Error] (Network) Failed to active ethernet.");
#endif
        return false;
    }
#ifdef DEBUG
    Serial.println("[Info] (Network) Ethernet activated.");
#endif
    return true;
}

bool Network::static_setup(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix)
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
        network_status = STATIC_SETUP_ACTIVE;
        return true;
    }
    else
    {
#ifdef DEBUG
        Serial.println("[Error] (Network) Static network setup failed!");
#endif
        network_status = STATIC_SETUP_FAILED;
        return false;
    }
}

bool Network::dhcp_setup()
{
    if (!ether.dhcpSetup())
    {

#ifdef DEBUG
        Serial.println("[Error] (Network) DHCP failed.");
#endif
        network_status = DHCP_SETUP_FAILED;
        return false;
    }

#ifdef DEBUG
    Serial.println("[Info] (Network) DHCP setup successful!");
    ether.printIp("[Info] IP:   ", ether.myip);
    ether.printIp("[Info] GW:   ", ether.gwip);
    ether.printIp("[Info] DNS:  ", ether.dnsip);
    ether.printIp("[Info] NM:   ", ether.netmask);
#endif
    network_status = DHCP_SETUP_ACTIVE;
    return true;
}

int8_t Network::get_network_status()
{
    return network_status;
}

void Network::sendNTPpacket(const uint8_t *remoteAddress)
{
    // buffer to hold outgoing packet
    byte packetBuffer[NTP_PACKET_SIZE];

    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    // Initialize values needed to for NTP request
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

void Network::udpReceiveNtpPacket(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *packetBuffer, uint16_t len)
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

bool Network::set_ntpserver(char *ntp_server)
{

    Serial.println(ntp_server);

    if (!ether.dnsLookup(ntp_server, true))
        Serial.println("DNS failed");

    uint8_t ntpIp[IP_LEN];
    ether.copyIp(ntpIp, ether.hisip);

    ether.udpServerListenOnPort(&udpReceiveNtpPacket, NTP_LOCALPORT);
    Serial.println("Started listening for response.");
    ether.packetLoop(ether.packetReceive());

    sendNTPpacket(ntpIp);
}

BufferFiller bfill;

bool Network::http_response(byte day, byte month, byte year, byte hour, byte minute, byte second)
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

#ifdef DEBUG
// Print networking config - only for debugging
bool Network::print_networking_config()
{
    ether.printIp(ether.myip);
    ether.printIp(ether.dnsip);
    ether.printIp(ether.gwip);
    ether.printIp(ether.netmask);
}
#endif

uint8_t *Network::get_ip()
{
    return ether.myip;
}

uint8_t *Network::get_gw()
{
    return ether.gwip;
}

uint8_t *Network::get_dns()
{
    return ether.dnsip;
}

uint8_t *Network::get_snm()
{
    return ether.netmask;
}