#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"

void setup()
{
    Serial.begin(57600);

    Time::init_rtc_module();
    Time::init_alarm_interrupt();

    // Beeper
    pinMode(A2, OUTPUT);

    Time::set_alarm_types(0, 0xAAAAAAAA);
    Time::set_alarm_types(1, 0xF0F0F0F0);
    Time::set_alarm_types(2, 0xAF00FF0A);

    // Add alarm
    Time::add_alarm(20, 54, 0);
    Time::add_alarm(20, 56, 1);
    Time::add_alarm(21, 0, 2);
}




char time_strings[64][9];

void print_time_info()
{
    // Function for debuggin purpose 
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
}
