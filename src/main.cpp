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

void network_manager()
{
    uint8_t NetworkSetup = Network::NetworkStatus;
    if (Network::init_ethernet())
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) Ethernet Modul wurde aktiviert.");
#endif
        if (Network::is_cable_connected())
        {
#ifdef DEBUG
            Serial.println("[Info] (Main) Netzwerkanschluss ist aktiv.");
#endif

            if (NetworkSetup == ETHERNET_DHCP_INIT)
            {
                Network::dhcp_setup();
                if (Network::NetworkStatus == ETHERNET_DHCP_SUCCESS)
                {
                    Storage::save_network_settings(Network::get_ip(), Network::get_gw(), Network::get_dns(), Network::get_prefix());
                    Storage::save_network_dhcp(true);
                }
            }
            else if (NetworkSetup == ETHERNET_STATIC_INIT)
            {
                Storage::read_network_settings(ip, gw, dns, &prefix);
                Network::static_setup(ip, gw, dns, prefix);
                Storage::save_network_dhcp(false);
            }
        }
    }
}

void setup()
{
    Serial.begin(9600);

    GUI::init_display();
#ifdef DEBUG
    Serial.println("[Info] (Main) Display wurde aktiviert.");
#endif
    if (Storage::read_network_dhcp())
    {
        Network::NetworkStatus = ETHERNET_DHCP_INIT;
    }
    else
    {
        Network::NetworkStatus = ETHERNET_STATIC_INIT;
    }
    network_manager();

    alarm_count = Storage::read_alarm_count();
    for (uint8_t i = 0; i < alarm_count; i++)
    {
        Storage::read_alarm(&alarms[i].minutes, &alarms[i].type, i);
    }

// Breaks DHCP setup ... 
//     TimeSync::init_timesync(Storage::read_network_ntp());
// #ifdef DEBUG
//     Serial.println("[Info] (Main) Zeitsynchronisation wurde aktiviert.");
// #endif

    Time::init_alarm_interrupt();
#ifdef DEBUG
    Serial.println("[Info] (Main) Timerinterrupt wurde konfiguriert.");
#endif

    // Beeper
    pinMode(OUTPUT_PIN, OUTPUT);
}

int navigation;
int last_navigation;
int selection;
bool refresh;

char time_string[9];
char buffer[20];
char compare_time_string[9];
char alarm_string[64][6];

void navigation_handler()
{
    switch (navigation)
    {
    case MENU:
    {

#ifdef DEBUG
        Serial.println("[Info] (Main) Menu");
#endif
        navigation = GUI::check_menu();

        if (navigation == TIME)
        {
            GUI::update_time(false);
        }
        break;
    }
    case TIMEPLAN:
    {
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
    }
    case TIME:
    {

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
    }
    case TIME_SETTING:
    {

        navigation = GUI::check_time_setting(time_string);

        if (navigation == BUTTON_ACCEPT)
        {
            uint8_t timearray[3];
            Time::timestring_to_timearray(time_string, timearray);
            Time::set_datetime(23, 18, 2, timearray[0], timearray[1], timearray[2]);
            navigation = TIME;
        }
        break;
    }
    case SYSTEM:
    {
        break;
    }
    case NETWORK_MENU:
    {
        navigation = GUI::check_network_menu();
        break;
    }
    case NETWORK_DHCP:
    {
        selection = GUI::check_network_dhcp();

        if (selection == NETWORK_DHCP_SWITCH)
        {
            if ((Network::NetworkStatus == ETHERNET_DHCP_SUCCESS) || (Network::NetworkStatus == ETHERNET_DHCP_FAILED))
            {
                Network::NetworkStatus = ETHERNET_STATIC_INIT;
            }
            else if ((Network::NetworkStatus == ETHERNET_STATIC_SUCCESS) || (Network::NetworkStatus == ETHERNET_STATIC_FAILED))
            {
                Network::NetworkStatus = ETHERNET_DHCP_INIT;
            }

            GUI::network_dhcp(Network::NetworkStatus, Network::get_ip(), Network::get_gw(), Network::get_dns(), Network::get_prefix());
            network_manager();
            GUI::network_dhcp(Network::NetworkStatus, Network::get_ip(), Network::get_gw(), Network::get_dns(), Network::get_prefix());
        }
        else if (selection == NETWORK_MENU)
        {
            navigation = NETWORK_MENU;
        }
        break;
    }
    case ALARM_CONFIG:
    {

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
    }
    case NEW_ALARM_CONFIG:
    {

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
    }
    case NETWORK_NTP:
    {
        selection = GUI::check_network_ntp(TimeSync::EnableNtpSync);
        if (selection == BUTTON_BACK)
        {
            navigation = NETWORK_MENU;
        }
        else if (selection == NETWORK_NTP_SWITCH)
        {
            TimeSync::EnableNtpSync = !TimeSync::EnableNtpSync;
            Storage::save_network_ntp(TimeSync::EnableNtpSync);
            refresh = true;
        }
        break;
    }
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
    {
        GUI::menu();
        break;
    }
    case TIMEPLAN:
    {
        Time::sort_alarms(alarms, alarm_count);
        Time::get_alarms_strings(alarms, alarm_count, alarm_string);
        GUI::timeplan(alarm_string, alarm_count);
        break;
    }
    case TIME:
    {
        if (Time::get_current_timestring(time_string))
        {
            GUI::time(time_string);
        }
        refresh = true;
        break;
    }
    case TIME_SETTING:
    {

        GUI::time_setting(time_string);
        break;
    }

    case ALARM_CONFIG:
    {
        alarm.minutes = alarms[selection].minutes;
        alarm.type = alarms[selection].type;
        Time::get_alarm_string(alarm.minutes, time_string);
        GUI::alarm_config(time_string, alarm.type);
        break;
    }
    case NEW_ALARM_CONFIG:
    {
        GUI::alarm_config((char *)"??:??", alarm.type);
        break;
    }
    case NETWORK_MENU:
    {
        GUI::network_menu();
        break;
    }
    case NETWORK_DHCP:
    {
        GUI::network_dhcp(Network::NetworkStatus, Network::get_ip(), Network::get_gw(), Network::get_dns(), Network::get_prefix());
        break;
    }
    case NETWORK_NTP:
    {
        Time::get_formatted_time(TimeSync::getLastNtpSync(), buffer);
        GUI::network_ntp(buffer, TimeSync::EnableNtpSync);
        break;
    }
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
