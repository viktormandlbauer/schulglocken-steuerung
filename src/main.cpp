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
    Serial.begin(9600);

    init_rtc_module();
    init_display();
}

void loop()
{
    String timeformat = "hh:mm";

    draw_time(get_time_string(timeformat));
    delay(1000);
}
