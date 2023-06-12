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
static bool alarms_enabled = true;

char alarm_string[MAXIMUM_AMOUNT_ALARMS][6];
int navigation = GUI_INIT;
int last_navigation = GUI_INIT;
int selection;
bool refresh;

char date_string[11];
char time_string[9];
char day_string[4];

char buffer[20];
char compare_time_string[9];
char addresses_string[49];

Time::AlarmException alarm_exceptions[MAXIMUM_AMOUNT_EXCEPTIONS];
uint8_t alarm_exceptions_count = 0;
char exception_start_string[6];
char exception_end_string[6];
char exception_buffer[MAXIMUM_AMOUNT_EXCEPTIONS][13];
bool reoccurring[MAXIMUM_AMOUNT_EXCEPTIONS];
bool reoccuring_exception;
uint8_t weekday_exception_list;

void navigation_handler()
{
    switch (navigation)
    {
    case STANDARD:
    {

        navigation = GUI::check_default_menu();
        break;
    }
    case MENU:
    {

#ifdef DEBUG
        Serial.println(F("[Info] (Main) Menu"));
#endif
        navigation = GUI::check_menu();
        break;
    }
    case TIMEPLAN:
    {
#ifdef DEBUG
        Serial.println(F("[Info] (Main) Timeplan"));
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
        if (navigation == BUTTON_MODIFY)
        {
            navigation = TIME_SETTING;
        }
        break;
    }
    case TIME_SETTING:
    {
        navigation = GUI::check_time_setting(buffer);

        if (navigation == BUTTON_ACCEPT)
        {
            Time::set_datetime(buffer);
            navigation = TIME;
        }
        break;
    }
    case SYSTEM:
    {
        navigation = GUI::check_exception_menu();
        if (navigation == BUTTON_BACK)
        {
            navigation = MENU;
        }
        break;
    }
    case REMOVE_EXCEPTION:
    {
        uint8_t *remove_at_index;
        navigation = GUI::check_remove_exceptions(exception_buffer, reoccurring, alarm_exceptions_count, remove_at_index);
        if (navigation == BUTTON_BACK)
        {
            navigation = SYSTEM;
        }
        else if (navigation == BUTTON_DELETE)
        {
            alarm_exceptions_count = Time::remove_alarm_exception_at_index(alarm_exceptions, alarm_exceptions_count, *remove_at_index);

            if (Time::check_alarm_exception(alarm_exceptions, alarm_exceptions_count, weekday_exception_list))
            {
                alarms_enabled = false;
            }
            else
            {
                alarms_enabled = true;
            }

            // Ausnahme aus EEPROM entfernen
            for (uint8_t i = 0; i < alarm_exceptions_count; i++)
            {
                Storage::save_exception(alarm_exceptions[i].BeginDay, alarm_exceptions[i].BeginMonth, alarm_exceptions[i].EndDay, alarm_exceptions[i].EndMonth, alarm_exceptions[i].reoccurring, i);
            }
            Storage::save_exception_count(alarm_exceptions_count);

            navigation = REMOVE_EXCEPTION;
            refresh = true;
        }
        break;
    }
    case ADD_EXCEPTION:
    {
        navigation = GUI::check_add_exception(exception_start_string, exception_end_string, &reoccuring_exception);
        if (navigation == BUTTON_BACK)
        {
            navigation = SYSTEM;
        }
        else if (navigation == BUTTON_ACCEPT)
        {
            alarm_exceptions_count = Time::add_alarm_exception(alarm_exceptions, Time::parse_to_alarm_exception(exception_start_string, exception_end_string, reoccuring_exception), alarm_exceptions_count);

            if (Time::check_alarm_exception(alarm_exceptions, alarm_exceptions_count, weekday_exception_list))
            {
                alarms_enabled = false;
            }
            else
            {
                alarms_enabled = true;
            }

            // Ausnahme zu EEPROM hinzufügen
            for (uint8_t i = 0; i < alarm_exceptions_count; i++)
            {
                Storage::save_exception(alarm_exceptions[i].BeginDay, alarm_exceptions[i].BeginMonth, alarm_exceptions[i].EndDay, alarm_exceptions[i].EndMonth, alarm_exceptions[i].reoccurring, i);
            }
            Storage::save_exception_count(alarm_exceptions_count);

            navigation = REMOVE_EXCEPTION;
        }
        break;
    }
    case WEEKDAY_EXCEPTION:
    {
        navigation = GUI::check_weekdays_exceptions(&weekday_exception_list);
        if (navigation == BUTTON_BACK)
        {
            Storage::save_weekday_exceptions(weekday_exception_list);
            if (Time::check_alarm_exception(alarm_exceptions, alarm_exceptions_count, weekday_exception_list))
            {
                alarms_enabled = false;
            }
            else
            {
                alarms_enabled = true;
            }
            navigation = SYSTEM;
        }
        break;
    }
    case NETWORK_MENU:
    {
        navigation = GUI::check_network_menu();
        break;
    }
    case NETWORK_IP:
    {
        selection = GUI::check_network_ip(&Network::nw_status.DhcpEnabled);

        if (selection == NETWORK_SETUP)
        {
            if (!Network::nw_status.linkup)
            {
                Network::check_link();
                refresh = true;
            }
            else if (Network::nw_status.DhcpEnabled)
            {
                Network::dhcp_setup();
                if (!Network::nw_status.error_code)
                {
                    Storage::save_network_dhcp(true);
                }
                else
                {
                }
                refresh = true;
            }
        }
        else if (selection == BUTTON_BACK)
        {
            navigation = NETWORK_MENU;
        }
        else if (selection == NETWORK_STATIC_MODIFY)
        {
            navigation = NETWORK_STATIC_MODIFY;
        }
        break;
    }
    case NETWORK_STATIC_MODIFY:
    {
        selection = GUI::check_network_ip_static(addresses_string);
        if (selection == BUTTON_ACCEPT)
        {
            sscanf(addresses_string, "%3hhu.%3hhu.%3hhu.%3hhu/%2hhu%3hhu.%3hhu.%3hhu.%3hhu%3hhu.%3hhu.%3hhu.%3hhu",
                   &Network::nw_status.ip[0], &Network::nw_status.ip[1], &Network::nw_status.ip[2], &Network::nw_status.ip[3], &Network::nw_status.snm,
                   &Network::nw_status.gw[0], &Network::nw_status.gw[1], &Network::nw_status.gw[2], &Network::nw_status.gw[3],
                   &Network::nw_status.dns[0], &Network::nw_status.dns[1], &Network::nw_status.dns[2], &Network::nw_status.dns[3]);
            Storage::save_network_settings(Network::nw_status.ip, Network::nw_status.gw, Network::nw_status.dns, Network::nw_status.snm);
            Storage::save_network_dhcp(false);
            Network::static_setup(Network::nw_status.ip, Network::nw_status.gw, Network::nw_status.dns, Network::nw_status.snm);
            navigation = NETWORK_IP;
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
                Serial.println(F("[Info] (Main) Successfully added new alarm!"));
#endif
                navigation = TIMEPLAN;
            }
            else
            {
#ifdef DEBUG
                Serial.println(F("[Error] (Main) Failed adding new alarm, because timeframe alarm already exists!"));
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
        else if (selection == NETWORK_NTP_TEST)
        {
            TimeSync::init_timesync(true);
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
        Serial.print(F("[Info] (Main) Action Handler - Last Navigation ID: "));
        Serial.println(last_navigation);
        Serial.print(F("[Info] (Main) Action Handler - New Navigation ID: "));
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
    Serial.print(F("[Info] (Main) Refresh Handler for navigation: "));
    Serial.println(navigation);
#endif
    switch (navigation)
    {
    case STANDARD:
    {

        Time::get_current_timestring(time_string);
        Time::get_current_date(date_string);
        Time::get_current_weekday(day_string);
        Time::get_upcoming_alarm_strings(alarms, alarm_count, alarm_string, 3);
        // TODO Upcomfing Exceptions

        GUI::default_menu(date_string, time_string, day_string, alarm_string, "01.01", "02.02", 0b11111111);
        break;
    }
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
        if (Time::get_current_datetime(buffer))
        {
            GUI::time(buffer);
        }
        refresh = true;
        break;
    }
    case SYSTEM:
    {
        GUI::exception_menu();
        break;
    }
    case REMOVE_EXCEPTION:
    {
        Time::get_alarm_exceptions_strings(alarm_exceptions, alarm_exceptions_count, exception_buffer, reoccurring);
        GUI::remove_exceptions(exception_buffer, reoccurring, alarm_exceptions_count);
        break;
    }
    case ADD_EXCEPTION:
    {
        reoccuring_exception = true;
        GUI::add_exception(exception_start_string, exception_end_string);
        break;
    }
    case WEEKDAY_EXCEPTION:
    {
        GUI::weekdays_exceptions(weekday_exception_list);
        break;
    }
    case TIME_SETTING:
    {
        GUI::time_setting(buffer);
        break;
    }
    case ALARM_CONFIG:
    {
        alarm.minutes = alarms[selection].minutes;
        alarm.type = alarms[selection].type;
        Time::get_alarm_string(alarm.minutes, time_string);
        GUI::alarm_config(time_string, alarm.type, false);
        break;
    }
    case NEW_ALARM_CONFIG:
    {
        GUI::alarm_config((char *)"??:??", alarm.type, true);
        break;
    }
    case NETWORK_MENU:
    {
        GUI::network_menu();
        break;
    }
    case NETWORK_IP:
    {
        Network::check_link();
        GUI::network_ip(Network::nw_status.DhcpEnabled, Network::nw_status.linkup, Network::nw_status.error_code, Network::get_ip(), Network::get_gw(), Network::get_dns(), Network::get_prefix());
        break;
    }
    case NETWORK_STATIC_MODIFY:
    {
        sprintf(addresses_string, "%03d.%03d.%03d.%03d/%02d%03d.%03d.%03d.%03d%03d.%03d.%03d.%03d",
                Network::nw_status.ip[0], Network::nw_status.ip[1], Network::nw_status.ip[2], Network::nw_status.ip[3],
                Network::nw_status.snm,
                Network::nw_status.gw[0], Network::nw_status.gw[1], Network::nw_status.gw[2], Network::nw_status.gw[3],
                Network::nw_status.dns[0], Network::nw_status.dns[1], Network::nw_status.dns[2], Network::nw_status.dns[3]);

        GUI::network_ip_static(addresses_string);
        break;
    }

    case NETWORK_NTP:
    {
        Time::get_formatted_datetime(TimeSync::LastNtpSync, buffer);
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

    if (Time::day_changed())
    {
        Time::reset_alarms(alarms, alarm_count);
        if (Time::check_alarm_exception(alarm_exceptions, alarm_exceptions_count, weekday_exception_list))
        {
            alarms_enabled = false;
        }
        else
        {
            alarms_enabled = true;
        }
    }

    if (alarms_enabled)
    {
        Time::check_alarms(alarms, alarm_count);
    }

    if (Network::nw_status.HttpEnabled)
    {
        Time::get_current_datetime(buffer);
        Network::http_response(buffer);
    }
}

void setup()
{
    Network::nw_status.HttpEnabled = true;
    Serial.begin(BAUD_RATE);

    pinMode(LED_SIGNAL, OUTPUT);
    pinMode(RELAIS, OUTPUT);
    pinMode(AUDIO_SIGNAL, OUTPUT);
    digitalWrite(LED_SIGNAL, HIGH);
    GUI::init_display();
#ifdef DEBUG
    Serial.println(F("[Info] (Main) Display wurde aktiviert."));
#endif

    Time::init_rtc_module();
#ifdef DEBUG
    Serial.println("[Info] (Main) RTC Modul wurde aktiviert.");
#endif
    Network::init_ethernet();
    Network::check_link();
    Network::nw_status.DhcpEnabled = Storage::read_network_dhcp();
    Storage::read_network_settings(Network::nw_status.ip, Network::nw_status.gw, Network::nw_status.dns, &Network::nw_status.snm);

    if (Network::nw_status.active && Network::nw_status.linkup)
    {
        if (Network::nw_status.DhcpEnabled)
        {
            Network::dhcp_setup();
        }
        else
        {
            Network::static_setup(Network::nw_status.ip, Network::nw_status.gw, Network::nw_status.dns, Network::nw_status.snm);
        }
    }

    TimeSync::init_timesync(Storage::read_network_ntp());

#ifdef DEBUG
    Serial.println("[Info] (Main) Zeitsynchronisation wurde aktiviert.");
#endif
    // Alarme von EEPROM auslesen
#ifdef DEBUG
    Serial.println("[Info] (Main) Reading alarms from EEPROM.");
#endif

    alarm_count = Storage::read_alarm_count();
    for (uint8_t i = 0; i < alarm_count; i++)
    {
        Storage::read_alarm(&alarms[i].minutes, &alarms[i].type, i);
    }

    // Alarmausnahmen von EEPROM auslesen
#ifdef DEBUG
    Serial.println("[Info] (Main) Reading alarm exceptions from EEPROM.");
#endif
    alarm_exceptions_count = Storage::read_exception_count();
    for (uint8_t i = 0; i < alarm_exceptions_count; i++)
    {
        Storage::read_exception(&alarm_exceptions[i].BeginDay, &alarm_exceptions[i].BeginMonth, &alarm_exceptions[i].EndDay, &alarm_exceptions[i].EndMonth, &alarm_exceptions[i].reoccurring, i);
    }

#ifdef DEBUG
    Serial.println("[Info] (Main) Reading weekday exceptions from EEPROM.");
#endif
    // Wochentagausnahmen von EEPROM auslesen
    weekday_exception_list = Storage::read_weekday_exceptions();

    Time::init_alarm_interrupt();
#ifdef DEBUG
    Serial.println(F("[Info] (Main) Timerinterrupt wurde konfiguriert."));
#endif
    digitalWrite(LED_SIGNAL, LOW);

    // Zeichnen von initialen Menü
    refresh_handler();
}