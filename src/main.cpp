// Libraries
//#include <Arduino.h>
//#include <Adafruit_GFX.h>
//#include <Waveshare_ILI9486.h>
//#include <EtherCard.h>
//#include <Wire.h>
//#include <RTClib.h>

// PIN Connections (Using Arduino UNO):
//   VCC -   3.3V
//   GND -    GND
//   SCK - Pin 13
//   SO  - Pin 12
//   SI  - Pin 11
//   CS  - Pin  8

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
