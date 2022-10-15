#include <Arduino.h>
#include <EtherCard.h>

class Network
{
private:
    byte mac_address[6] = {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};
    bool dhcp;
    byte *ip_address;
    byte *gw_address;
    byte *subnetmask;
    byte *dns_address;

public:
    bool connected;

    Network();
    Network(byte ip_address[4], byte gw_address[4], byte dns_address[4], byte subnetmask[4]);

    void set_ip(byte ip_address[4]);
    byte *get_ip();
    void set_gw(byte gw_address[4]);
    byte *get_gw();
    void set_dns(byte dns_address[4]);
    byte *get_dns();
    void set_subnetmask(byte subnetmask[4]);
    byte *get_subnetmask();
    bool init_network();
    bool test_gateway();
    bool test_ntp();
};