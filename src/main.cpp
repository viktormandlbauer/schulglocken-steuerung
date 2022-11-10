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
#include "myutil.h"

void setup()
{
    Serial.begin(57600);

    Time::init_rtc_module();

    GUI::init_display();
    // GUI::draw_button();

    Network::init_ethernet();

    Network::init_dhcp_setup();
    // uint8_t ip[] = {192, 168, 178, 48};
    // uint8_t gw[] = {192, 168, 178, 1};
    // uint8_t dns[] = {192, 168, 178, 1};
    // uint8_t snm[] = {255, 255, 255, 0};

    // Network::init_static_setup(ip, gw, dns, snm);
}

void loop()
{
    char buf[] = "hh:mm:ss";

    String time = Time::get_time_string(buf);

    GUI::draw_time(time);

    //Serial.println(myutil::create_http_response_time(time));

    Network::show_http_status(myutil::create_http_response_time(time));
}
