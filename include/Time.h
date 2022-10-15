#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

void init_rtc_module();
char *get_time_string(String format);
