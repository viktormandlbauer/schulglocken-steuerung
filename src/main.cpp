#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"

void setup()
{
    Serial.begin(57600);

    Time::init_rtc_module();
}

char time_string[9];

void loop()
{
    Time::get_time(time_string);
    Serial.println(time_string);
    delay(1000);
}
