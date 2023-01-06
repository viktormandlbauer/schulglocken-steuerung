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

    Time::add_alarm(21, 45, 1);
    Time::add_alarm(21, 50, 1);
}

char time_strings[100][9];

void print_time_info()
{
    Serial.println("Current time:");
    Time::get_current_timestring(time_strings[0]);
    Serial.println(time_strings[0]);

    Serial.println("Current alarms:");
    Time::get_alarms_strings(time_strings);
    for (int i = 0; i < Time::get_alarm_count(); i++)
    {
        Serial.println(time_strings[i]);
    }
}

void loop()
{
    print_time_info();
    delay(1000);
}
