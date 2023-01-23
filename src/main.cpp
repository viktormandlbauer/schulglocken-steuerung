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

    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 0, 6, 1);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 0, 7, 0);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 21, 5, 2);

    Storage::save_alarms(alarms, alarms_type_assignment, alarm_count);
    // alarm_count = Storage::read_alarms(alarms, alarms_type_assignment);

    char full_time_string[9];
    Time::get_current_timestring(full_time_string);
    Serial.print("Current time:");
    Serial.println(full_time_string);

    char time_string[10][6];
    Time::get_alarms_strings(alarms, alarm_count, time_string);

    Serial.println("Timestrings: ");
    for (int i = 0; i < alarm_count; i++)
    {
        Serial.println(time_string[i]);
    }
}

void test2()
{
    // uint8_t ip[] = {192, 168, 0, 52};
    // uint8_t dns[] = {192, 168, 0, 1};
    // uint8_t gw[] = {192, 168, 0, 1};
    // uint8_t prefix = 24;
    // Network::static_setup(ip, gw, dns, prefix);
    // Storage::save_network_settings(ip, gw, dns, prefix);
    uint8_t ip[4];
    uint8_t dns[4];
    uint8_t gw[4];
    uint8_t prefix;
    Storage::read_network_settings(ip, dns, gw, &prefix);
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

    Network::init_ethernet();
#ifdef DEBUG
    Serial.println("[Info] (Main) Ethernet wurde aktiviert.");
#endif

        // Beeper
        pinMode(A2, OUTPUT);

    test1();
    test2();
}

void loop()
{
    if (GUI::display_action())
    {
        action_handler();
    }
    Time::check_alarm(alarms, alarms_type_assignment, alarm_count);
}
