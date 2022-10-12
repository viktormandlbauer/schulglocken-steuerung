// Libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <EtherCard.h>
#include <Wire.h>
#include <RTClib.h>

#include "Time.h"
#include "GUI.h"

void setup()
{
    Serial.begin(9600);

    init_rtc_module();
    init_display();
}

void loop()
{
    draw_time(get_time_string("hh:mm"));
    delay(1000);
}
