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

// Alarme
static Time::Alarm alarms[MAXIMUM_AMOUNT_ALARMS];
static Time::Alarm alarm;
static uint8_t alarm_count = 0;

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

    // Alarmtypes
    Time::set_alarm_types(0, 0xAAAAAAAA);
    Time::set_alarm_types(1, 0xF0F0F0F0);
    Time::set_alarm_types(2, 0xAF00FF0A);

    // Beeper
    pinMode(A2, OUTPUT);
}

int navigation;
int last_navigation;
int selection;
bool refresh = false;

char time_string[9];
char alarm_string[64][6];

void navigation_handler()
{

    switch (navigation)
    {
    case MENU:
#ifdef DEBUG
        Serial.println("[Info] (Main) Menu");
#endif
        navigation = GUI::check_menu();
        break;

    case TIMEPLAN:
#ifdef DEBUG
        Serial.println("[Info] (Main) Timeplan");
#endif
        selection = GUI::check_timeplan(alarm_count);

        if (selection < MAXIMUM_AMOUNT_ALARMS)
        {
            navigation = ALARM_CONFIG;
        }
        else if (selection == BUTTON_UP || selection == BUTTON_DOWN)
        {
            GUI::draw_alarm_list(alarm_string);
        }
        else if (selection == BUTTON_ADD)
        {
            navigation = NEW_ALARM_CONFIG;
        }
        else if (selection == BUTTON_BACK)
        {
            navigation = MENU;
        }
        else if (selection == NO_CHANGE)
        {
            navigation = TIMEPLAN;
        }
        break;

    case TIME:
        break;

    case SYSTEM:
        break;

    case NETWORK:
        break;

    case ALARM_CONFIG:
#ifdef DEBUG
        Serial.println("[Info] (Main) Active alarm setting");
#endif

        navigation = GUI::check_alarm_config(&alarm.minutes, &alarm.type, false);
        if (navigation == BUTTON_ACCEPT)
        {
            if (alarm.minutes == alarms[selection].minutes && alarm.type == alarms[selection].type)
            {

#ifdef DEBUG
                Serial.println("[Info] (Main) Alarm didn't change.");
#endif
                navigation = TIMEPLAN;
            }
            else
            {
                if (Time::alarm_exists(alarms, alarm.minutes, alarm_count))
                {
#ifdef DEBUG
                    Serial.println("[Error] (Main) Can't update alarm because timeframe already exists.");
#endif
                    navigation = ALARM_CONFIG;
                }
                else
                {
                    alarms[selection].minutes = alarm.minutes;
                    alarms[selection].type = alarm.type;
#ifdef DEBUG
                    Serial.println("[Info] (Main) Successfully updated alarm.");
#endif
                    navigation = TIMEPLAN;
                }
            }
        }
        break;

    case NEW_ALARM_CONFIG:
        navigation = GUI::check_alarm_config(&alarm.minutes, &alarm.type, true);

        if (navigation == BUTTON_ACCEPT)
        {
            if (Time::add_alarm(alarms, &alarm_count, alarm.minutes, alarm.type))
            {
#ifdef DEBUG
                Serial.println("[Info] (Main) Successfully added new alarm!");
#endif
                navigation = TIMEPLAN;
            }
            else
            {
#ifdef DEBUG
                Serial.println("[Error] (Main) Failed adding new alarm, because timeframe alarm already exists!");
#endif
                navigation = NEW_ALARM_CONFIG;
            }
        }
        break;

    default:
        break;
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
    refresh = false;
#ifdef DEBUG
    Serial.println("[Info] (Main) Refresh GUI");
#endif

    switch (navigation)
    {
    case MENU:
        GUI::menu();
        break;

    case TIMEPLAN:
        Time::sort_alarms(alarms, alarm_count);
        Time::get_alarms_strings(alarms, alarm_count, alarm_string);
        GUI::timeplan(alarm_string);
        break;

    case ALARM_CONFIG:
        alarm.minutes = alarms[selection].minutes;
        alarm.type = alarms[selection].type;
        Serial.println(alarms[selection].minutes);
        Time::get_alarm_string(alarm.minutes, time_string);
        GUI::alarm_config(time_string, alarm.type);
        break;

    case NEW_ALARM_CONFIG:
        GUI::alarm_config((char *)"??:??", alarm.type);
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

    Time::check_alarms(alarms, alarm_count);

    // Check Network
}
