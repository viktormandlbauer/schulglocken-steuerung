#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

namespace Time
{
    bool init_rtc_module();

    // Return time
    String get_time_string(String format);
    int *get_time_values(int arr[]);
}
