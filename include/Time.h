#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <TimeLib.h>
#include <LinkedList.h>

namespace Time
{
    bool init_rtc_module();

    // Return time
    String get_time_string(String format);
    
}
