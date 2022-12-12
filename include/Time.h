#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <TimeLib.h>
#include <SPI.h>

namespace Time
{
    bool init_rtc_module();
    alert add_alert();
    time_t time_provider();
}
