#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"

void setup()
{
    Serial.begin(57600);

    Time::init_rtc_module();
}

void loop()
{
}
