#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

void init_rtc_module();
String get_time_string(String format);
