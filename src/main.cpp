#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"

void setup()
{
    Serial.begin(57600);

    Time::init_rtc_module();

    // Beeper
    pinMode(A2, OUTPUT);

    Time::add_alarm(17, 1, 1);
    Time::add_alarm(17, 3, 1);
    Time::add_alarm(17, 5, 1);
    Time::add_alarm(16, 57, 1);
    Time::add_alarm(16, 59, 1);
}

char time_strings[64][9];

void print_time_info()
{
    Serial.println("Current time:");
    Time::get_current_timestring(time_strings[0]);
    Serial.println(time_strings[0]);

    Serial.println("Current alarms:");
    uint8_t alarm_count = Time::get_alarms_strings(time_strings);
    for (int i = 0; i < alarm_count; i++)
    {
        Serial.println(time_strings[i]);
    }
}

void loop()
{

    Time::check_alarm();

    delay(1000);
}
