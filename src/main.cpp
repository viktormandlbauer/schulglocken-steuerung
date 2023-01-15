// Einstellung für den DEBUG Mode
#include "DEFINITIONS.h"
#ifdef DEBUG_MAIN
#define DEBUG
#endif

#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"
#include "Storage.h"

void setup()
{
    Serial.begin(57600);

    // Time::init_rtc_module();
#ifdef DEBUG
    Serial.println("[Info] (Main) RTC Modul wurde aktiviert.");
#endif

    Time::init_alarm_interrupt();
#ifdef DEBUG
    Serial.println("[Info] (Main) Timerinterrupt wurde konfiguriert.");
#endif

    GUI::init_display();
#ifdef DEBUG
    Serial.println("[Info] (Main) Display wurde aktiviert.");
#endif

    // Beeper
    pinMode(A2, OUTPUT);
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

void test1()
{
    Time::set_alarm_types(0, 0xAAAAAAAA);
    Time::set_alarm_types(1, 0xF0F0F0F0);
    Time::set_alarm_types(2, 0xAF00FF0A);

    // Add alarm
    // Time::add_alarm(0, 1, 2);
    // Time::add_alarm(0, 10, 0);
    // Time::add_alarm(23, 59, 1);
    // Time::add_alarm(21, 0, 2);
    // Time::add_alarm(21, 0, 0);
    // Time::add_alarm(21, 0, 1);
    // Time::add_alarm(21, 0, 2);

    uint16_t *alarms;
    uint8_t *alarm_type_assignement;

    alarms = Time::get_alarms();
    alarm_type_assignement = Time::get_alarm_assignements();

    // Storage::save_alarms(alarms, alarm_type_assignement, Time::get_alarm_count());
    Time::set_alarm_count(Storage::read_alarms(alarms, alarm_type_assignement));
}

int navigation = 0;
bool check_action()
{
    if (navigation == 0)
    {
        navigation = GUI::check_menu();
    }
    else if (navigation == 1)
    {
        // Zeitplan
    }
    else if (navigation == 2)
    {
        // Uhrzeit
    }
    else if (navigation == 3)
    {
        // Systeminfo
    }
    else if (navigation == 4)
    {
        // Netzwerk
    }
}

void loop()
{
    if (GUI::display_action())
    {
    }
    Time::check_alarm();
}
