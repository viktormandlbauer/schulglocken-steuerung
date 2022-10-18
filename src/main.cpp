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

#include "Time.h"
#include "GUI.h"
#include "Network.h"

namespace
{
    Network nw;
}

void setup()
{
    Serial.begin(57600);

    init_rtc_module();
    init_display();

    draw_button();

    nw = Network();
    nw.init_network();
}

void loop()
{
    char buf[] = "hh:mm:ss";

    draw_time(get_time_string(buf));
    nw.get_ntp_time();
    nw.show_http_status();
}
