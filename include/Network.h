#include <Arduino.h>
#include <EtherCard.h>
#define CHIP_SELECT 28 // Mega
// #define CHIPSE_SELECT 14 // Uno

// Local UDP port to use

namespace Network
{
    const byte mac_address[6] = MAC_ADDRESS;
    const unsigned int NTP_LOCALPORT = 8888;
    const unsigned int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
    const unsigned int NTP_REMOTEPORT = 123; // NTP requests are to port 123

    // server 0.at.pool.ntp.org
    // server 1.at.pool.ntp.org
    // server 2.at.pool.ntp.org
    // server 3.at.pool.ntp.org
    // Network Configuration
    bool init_ethernet();
    int8_t get_network_status();
    bool static_setup(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t snm);
    bool dhcp_setup();

    uint8_t *get_ip();
    uint8_t *get_gw();
    uint8_t *get_dns();
    uint8_t *get_snm();

    // NTP
    bool set_ntpserver(String ntp_server);
    void udpReceiveNtpPacket(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *packetBuffer, uint16_t len);
    void sendNTPpacket(const uint8_t *remoteAddress);

    bool print_networking_config();
    char *resolve_ip_address();

    bool http_response(byte day, byte month, byte year, byte hour, byte minute, byte second);
    void set_warning(int type);
}