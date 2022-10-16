// Libraries
//#include <Arduino.h>
//#include <Adafruit_GFX.h>
//#include <Waveshare_ILI9486.h>
//#include <EtherCard.h>
//#include <Wire.h>
//#include <RTClib.h>

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

    nw = Network();
    nw.init_network();
}

void loop()
{
    char buf[] = "hh:mm:ss";

    draw_time(get_time_string(buf));

    nw.show_http_status();
}
