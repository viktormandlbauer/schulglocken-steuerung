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

void setup()
{
    Serial.begin(57600);

    init_rtc_module();
    init_display();

    Network nw = Network();
    nw.init_network();
}

void loop()
{
    char buf[] = "hh:mm:ss";

    draw_time(get_time_string(buf));
    delay(1000);
}
