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

char time_strings[64][9];

// Array mit den Alarmzeiten
static uint16_t alarms[MAXIMUM_AMOUNT_ALARMS] = {0};

// Array mit Alarmtypzuweisung
static uint8_t alarms_type_assignment[MAXIMUM_AMOUNT_ALARMS] = {0};

// Anzahl der Alarme
static uint8_t alarm_count = 0;

void test1()
{
    // Alarmtypes
    Time::set_alarm_types(0, 0xAAAAAAAA);
    Time::set_alarm_types(1, 0xF0F0F0F0);
    Time::set_alarm_types(2, 0xAF00FF0A);

    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 0, 5, 2);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 0, 6, 1);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 0, 7, 0);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 1, 8, 1);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 9, 9, 2);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 10, 10, 0);

    Storage::save_alarms(alarms, alarms_type_assignment, alarm_count);
    alarm_count = Storage::read_alarms(alarms, alarms_type_assignment);

    char time_string[10][6];
    Time::get_alarms_strings(alarms, alarm_count, time_string);

    Serial.println("Timestrings: ");
    for (int i = 0; i < alarm_count; i++)
    {
        Serial.println(time_string[i]);
    }
}

int navigation = 0;

char time_string[9];

void action_handler()
{
    if (navigation == 0)
    {
        navigation = GUI::menu();
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

void setup()
{
    Serial.begin(57600);

    Time::init_rtc_module();
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


    test1();
}

void loop()
{
    if (GUI::display_action())
    {
        action_handler();
    }
    Time::check_alarm(alarms, alarms_type_assignment, alarm_count);
}
