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

    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 1, 6, 1);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 5, 5, 2);
    alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 2, 7, 0);

    // Storage::save_alarms(alarms, alarms_type_assignment, alarm_count);
    // alarm_count = Storage::read_alarms(alarms, alarms_type_assignment);
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

void setup()
{
    Serial.begin(57600);

    GUI::init_display();
#ifdef DEBUG
    Serial.println("[Info] (Main) Display wurde aktiviert.");
#endif

    Time::init_rtc_module();
#ifdef DEBUG
    Serial.println("[Info] (Main) RTC Modul wurde aktiviert.");
#endif

    Time::init_alarm_interrupt();
#ifdef DEBUG
    Serial.println("[Info] (Main) Timerinterrupt wurde konfiguriert.");
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

int navigation;
int last_navigation;
int selection;
bool refresh = false;

char time_string[9];
char alarm_string[64][6];

bool navigation_handler()
{
    if (navigation == 0)
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) Menu");
#endif
        navigation = GUI::check_menu();
    }
    else if (navigation == 1)
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) Timeplan");
#endif

        selection = GUI::check_timeplan();

        if (selection < MAXIMUM_AMOUNT_ALARMS)
        {
            navigation = 5;
        }
        else if (selection == 252)
        {
            GUI::draw_alarm_list(alarm_string);
        }
        else if (selection == 253)
        {
            // back
            navigation = 6;
        }
        else if (selection == 254)
        {
            // Add Alarm
            navigation = 0;
        }
        else if (selection == 255)
        {
            navigation = 1;
        }
    }
    else if (navigation == 2)
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) Time");
#endif
        // Uhrzeit
    }
    else if (navigation == 3)
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) System");
#endif
        // Systeminfo
    }
    else if (navigation == 4)
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) Network ");
#endif
    }
    else if (navigation == 5)
    {

#ifdef DEBUG
        Serial.println("[Info] (Main) Active alarm setting");
#endif

        navigation = GUI::check_alarm_config(&alarms[selection]);
    }
    else if (navigation == 6)
    {
        Time::Alarm new_alarm = {0, 0, 0};

        alarm_count = Time::add_alarm(alarms, alarms_type_assignment, alarm_count, 0, 0, 0);
        navigation = GUI::check_alarm_config(&alarms[alarm_count]);

        // Prevents refreshing GUI, because its the same interface as navigation 5
        last_navigation = 5;
        navigation = 5;
    }

    // Eine Aktualisierung des Bildschirm ist nur notwendig,
    // wenn sich die Navigation geändert hat oder die Komponente Updates benötigt.
    if (navigation != last_navigation)
    {
#ifdef DEBUG
        Serial.print("[Info] (Main) Action Handler - Last Navigation ID: ");
        Serial.println(last_navigation);
        Serial.print("[Info] (Main) Action Handler - New Navigation ID: ");
        Serial.println(navigation);
#endif
        last_navigation = navigation;
        refresh = true;
    }
}

void refresh_handler()
{
#ifdef DEBUG
    Serial.println("[Info] (Main) Refresh GUI");
#endif

    switch (navigation)
    {
    case 0:
        GUI::menu();
        refresh = false;
        break;

    case 1:
        Time::get_alarms_strings(alarms, alarm_count, alarm_string);
        GUI::timeplan(alarm_string);
        refresh = false;
        break;

    case 5:
        Time::get_alarm_string(alarms[selection], time_string);
        GUI::alarm_config(time_string, &alarms_type_assignment[selection]);
        refresh = false;
        break;

    case 6:
        GUI::alarm_config("00:00", &alarms_type_assignment[selection]);
        refresh = false;
        break;
    }
}

void loop()
{
    if (GUI::display_action())
    {
        navigation_handler();
    }

    if (refresh)
    {
        refresh_handler();
    }

    Time::check_alarm(alarms, alarms_type_assignment, alarm_count);

    // Check Network
}
