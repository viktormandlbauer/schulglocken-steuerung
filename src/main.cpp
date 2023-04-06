#include "DEFINITIONS.h"
#ifdef DEBUG_MAIN
#define DEBUG
#endif

#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"
#include "Storage.h"
#include "TimeSync.h"

// Alarme
static Time::Alarm alarms[MAXIMUM_AMOUNT_ALARMS];
static Time::Alarm alarm;
static uint8_t alarm_count = 0;

uint8_t ip[4], gw[4], dns[4], prefix;

void setup()
{
    Serial.begin(57600);

    GUI::init_display();
#ifdef DEBUG
    Serial.println("[Info] (Main) Display wurde aktiviert.");
#endif

    Time::init_alarm_interrupt();
#ifdef DEBUG
    Serial.println("[Info] (Main) Timerinterrupt wurde konfiguriert.");
#endif
    Network::init_ethernet();
    if (!Storage::read_network_dhcp())
    {
        Network::dhcp_setup();
    }
    else
    {
        Storage::read_network_settings(ip, gw, dns, &prefix);
        Network::static_setup(ip, gw, dns, prefix);
    }

    Time::init_rtc_module();
#ifdef DEBUG
    Serial.println("[Info] (Main) RTC Modul wurde aktiviert.");
#endif

#ifdef DEBUG
    Serial.println("[Info] (Main) Ethernet wurde aktiviert.");
#endif

    alarm_count = Storage::read_alarm_count();
    for (uint8_t i = 0; i < alarm_count; i++)
    {
        Storage::read_alarm(&alarms[i].minutes, &alarms[i].type, i);
    }

    // Alarmtypes
    Time::set_alarm_types(0, 0xAAAAAAAA);
    Time::set_alarm_types(1, 0xF0F0F0F0);
    Time::set_alarm_types(2, 0xAF00FF0A);

    // Beeper
    pinMode(OUTPUT_PIN, OUTPUT);
}

int navigation;
int last_navigation;
int selection;
bool refresh;

char time_string[9];
char compare_time_string[9];
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

        if (navigation == TIME)
        {
            GUI::update_time(false);
        }
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
        navigation = GUI::check_time();
        if (navigation != TIME)
        {
            GUI::update_time(false);
        }
        if (navigation == BUTTON_MODIFY)
        {
            navigation = TIME_SETTING;
        }
        break;

    case TIME_SETTING:
        navigation = GUI::check_time_setting(time_string);

        if (navigation == BUTTON_ACCEPT)
        {
            uint8_t timearray[3];
            Time::timestring_to_timearray(time_string, timearray);
            Time::set_datetime(23, 18, 2, timearray[0], timearray[1], timearray[2]);
            navigation = TIME;
        }
        break;
    case SYSTEM:
        break;

    case NETWORK_MENU:
        navigation = GUI::check_network_menu();
        break;

    case NETWORK_CONFIG:
        int8_t network_status = Network::get_network_status();
        uint8_t ip[4], dns[4], gw[4];
        uint8_t snm;

        navigation = GUI::check_network_config(&network_status, ip, dns, gw, snm);
        if (navigation == BUTTON_MODIFY)
        {
            if (network_status == DHCP_SETUP_INIT)
            {

#ifdef DEBUG
                Serial.println("[Info] (Main) DHCP Mode from GUI ");
#endif

                if (Network::dhcp_setup())
                {
                    uint8_t *snm = Network::get_snm();
                    uint8_t snm_prefix = 0;
                    for (uint8_t i = 0; i < 4; i++)
                    {
                        for (uint8_t j = 0; j < 8; j++)
                        {
                            if (snm[i] && 0b1 << j)
                            {
                                snm_prefix += 1;
                            }
                        }
                    }
#ifdef DEBUG
                    Serial.print("[Info] (Main) Calculated SNM Prefix: ");
                    Serial.println(snm_prefix);
#endif
                    GUI::network_config(network_status, Network::get_ip(), Network::get_gw(), Network::get_dns(), snm_prefix);
                }
            }
            else if (network_status == STATIC_SETUP_INIT)
            {
#ifdef DEBUG
                Serial.println("[Info] (Main) Static Mode from GUI");
#endif
                // Network::static_setup(ip, dns, gw, snm);
            }

            if (network_status == STATIC_SETUP_FAILED || network_status == DHCP_SETUP_FAILED)
            {
            }
            else if (network_status == STATIC_SETUP_ACTIVE || network_status == DHCP_SETUP_ACTIVE)
            {
            }

            navigation = NETWORK_CONFIG;
        }
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
                    for (uint8_t i = 0; i < alarm_count; i++)
                    {
                        Storage::save_alarm(alarms[i].minutes, alarms[i].type, i);
                    }
                    Storage::save_alarm_count(alarm_count);

#ifdef DEBUG
                    Serial.println("[Info] (Main) Successfully updated alarm.");
#endif
                    navigation = TIMEPLAN;
                }
            }
        }
        else if (navigation == BUTTON_DELETE)
        {
            alarm_count = Time::remove_alarm_at_index(alarms, alarm_count, selection);

            for (uint8_t i = 0; i < alarm_count; i++)
            {
                Storage::save_alarm(alarms[i].minutes, alarms[i].type, i);
            }
            Storage::save_alarm_count(alarm_count);
            navigation = TIMEPLAN;
        }
        break;

    case NEW_ALARM_CONFIG:
        navigation = GUI::check_alarm_config(&alarm.minutes, &alarm.type, true);

        if (navigation == BUTTON_ACCEPT)
        {
            if (Time::add_alarm(alarms, &alarm_count, alarm.minutes, alarm.type))
            {
                for (uint8_t i = 0; i < alarm_count; i++)
                {
                    Storage::save_alarm(alarms[i].minutes, alarms[i].type, i);
                }
                Storage::save_alarm_count(alarm_count);
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
    // wenn sich die Navigation geändert hat oder die GUI Komponenten Updates benötigen.
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
    Serial.println("[Info] (Main) Refresh Handler");
#endif

    switch (navigation)
    {
    case MENU:
        GUI::menu();
        break;

    case TIMEPLAN:
        Time::sort_alarms(alarms, alarm_count);
        Time::get_alarms_strings(alarms, alarm_count, alarm_string);
        GUI::timeplan(alarm_string, alarm_count);
        break;

    case TIME:
        if (Time::get_current_timestring(time_string))
        {
            GUI::time(time_string);
        }
        refresh = true;
        break;

    case TIME_SETTING:
        GUI::time_setting(time_string);
        break;

    case ALARM_CONFIG:
        alarm.minutes = alarms[selection].minutes;
        alarm.type = alarms[selection].type;
        Time::get_alarm_string(alarm.minutes, time_string);
        GUI::alarm_config(time_string, alarm.type);
        break;

    case NEW_ALARM_CONFIG:
        GUI::alarm_config((char *)"??:??", alarm.type);
        break;

    case NETWORK_MENU:
        GUI::network_menu();
        break;

    case NETWORK_CONFIG:

        GUI::network_config(Network::get_network_status(), Network::get_ip(), Network::get_gw(), Network::get_dns(), Network::get_snm());
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
}
