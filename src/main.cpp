#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"

bool error_flag;

void setup()
{
    Serial.begin(57600);


    GUI::init_display();

    if (!Time::init_rtc_module())
    {
        GUI::draw_warning();
        // Network::set_warning();
    }

    if (Network::init_ethernet())
    {
        // Static network setup
        //Network::init_dhcp_setup();
        uint8_t ip[] = {192, 168, 178, 48};
        uint8_t gw[] = {192, 168, 178, 1};
        uint8_t dns[] = {192, 168, 178, 1};
        uint8_t snm[] = {255, 255, 255, 0};
        Network::init_static_setup(ip, gw, dns, snm);
    }
    else
    {
        GUI::draw_warning();
    }

    GUI::draw_button();
}

void loop()
{
    char buf[] = "hh:mm:ss";

    String time = Time::get_time_string(buf);
    GUI::draw_time(time);
}
