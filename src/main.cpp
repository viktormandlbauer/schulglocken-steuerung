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
    
    // Add alarm
    Time::add_alarm(20, 5, 0);
    Time::add_alarm(20, 6, 1);
    Time::add_alarm(20, 7, 2);
    Time::add_alarm(20, 8, 0);
    Time::add_alarm(20, 9, 1);
    Time::add_alarm(20, 10, 2);
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
