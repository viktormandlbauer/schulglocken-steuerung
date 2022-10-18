#include <Arduino.h>
#include <EtherCard.h>
#define CHIP_SELECT 14

// server 0.at.pool.ntp.org
// server 1.at.pool.ntp.org
// server 2.at.pool.ntp.org
// server 3.at.pool.ntp.org
const char NTP_REMOTEHOST[] PROGMEM = "0.at.pool.ntp.org"; // NTP server name

const unsigned int NTP_REMOTEPORT = 123; // NTP requests are to port 123
const unsigned int NTP_LOCALPORT = 8888; // Local UDP port to use
const unsigned int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

class Network
{
private:
    byte mac_address[6] = {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};
    bool dhcp;
    String *ntp_address;
    byte *ip_address;
    byte *gw_address;
    byte *subnetmask;
    byte *dns_address;

public:
    bool connected;

    Network();
    Network(byte ip_address[4], byte gw_address[4], byte dns_address[4], byte subnetmask[4]);

    void set_ip(byte ip_address[4]);
    void set_gw(byte gw_address[4]);
    void set_dns(byte dns_address[4]);
    void set_subnetmask(byte subnetmask[4]);
    void set_ntpadr(String ntp_server);

    byte *get_ip();
    byte *get_gw();
    byte *get_dns();
    byte *get_subnetmask();
    String *get_ntpsrv();

    bool init_network();
    bool check_network();
    bool test_gateway();
    bool dns_lookup();
    unsigned long get_ntp_time();
    bool show_http_status();
};