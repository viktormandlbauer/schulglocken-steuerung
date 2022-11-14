#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"
#include "Timequeue.h"

bool error_flag;

void setup()
{
    Serial.begin(57600);

    struct Timequeue::alert a1;
    a1.hour = 3;
    a1.minute = 4;
    a1.second = 5;
    struct Timequeue::alert a2;
    a2.hour = 7;
    a2.minute = 8;
    a2.second = 9;
    struct Timequeue::alert a3;
    a3.hour = 10;
    a3.minute = 11;
    a3.second = 12;
    struct Timequeue::alert a4;
    a4.hour = 13;
    a4.minute = 14;
    a4.second = 15;
    Timequeue::alert alerts[5];

    alerts[0] = a1;
    alerts[1] = a2;
    alerts[2] = a3;
    alerts[3] = a4;

    Timequeue::extend_array();

    GUI::init_display();

    if (!Time::init_rtc_module())
    {
        GUI::draw_warning();
        // Network::set_warning();
    }

    if (Network::init_ethernet())
    {
        Network::init_dhcp_setup();
        // Static network setup
        //  uint8_t ip[] = {192, 168, 178, 48};
        //  uint8_t gw[] = {192, 168, 178, 1};
        //  uint8_t dns[] = {192, 168, 178, 1};
        //  uint8_t snm[] = {255, 255, 255, 0};
        // Network::init_static_setup(ip, gw, dns, snm);
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

    int myvalues[3];
    int *times = Time::get_time_values(myvalues);

    Network::http_response(1, 2, 3, times[0], times[1], times[2]);
}
