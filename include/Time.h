#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

namespace Time
{
    void init_rtc_module();
    String get_time_string(String format);

}
