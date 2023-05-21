#include "DEFINITIONS.h"
#ifdef DEBUG_GUI
#define DEBUG
#endif

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <XPT2046_Touchscreen.h>

namespace GUI
{
    void init_display();
    bool display_action();
    bool check_input();

    void menu();
    uint8_t check_menu();

    void timeplan(char alarm_strings[][6], uint8_t alarm_count);
    void draw_alarm_list(char alarm_strings[][6]);
    uint8_t check_timeplan(uint8_t alarm_count);

    void alarm_config(char alarm_time[6], uint8_t alarm_type);
    uint8_t check_alarm_config(uint16_t *alarm, uint8_t *alarm_type, bool is_new);

    void time(char datetime_string[9]);
    uint8_t check_time();
    void time_setting(char time_string[9]);
    uint8_t check_time_setting(char *time_string);

    void network_menu();
    uint8_t check_network_menu();

    void network_ntp(char *lastNtpSync, bool isEnabled);
    uint8_t check_network_ntp(bool isEnabled);

    void network_ip(bool DhcpEnabled, bool IsLinkUP, uint8_t error_code, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);
    uint8_t check_network_ip(bool *DhcpEnabled);
    void network_ip_static(char addresses_string[49]);
    uint8_t check_network_ip_static(char addresses_string[49]);

    void exception_menu();
    uint8_t check_exception_menu();

    void add_exception(char exception_start_string[6], char exception_end_string[6]);
    uint8_t check_add_exception(char exception_start_string[6], char exception_end_string[6], bool *reoccurring);

    void weekdays_exceptions(uint8_t weekday_exception_list);
    uint8_t check_weekdays_exceptions(uint8_t *weekday_exception_list);

    void remove_exceptions(char alarm_exceptions_string[][13], bool reoccuring[], uint8_t alarm_exception_count);
    uint8_t check_remove_exceptions(char alarm_exceptions_string[][13], bool reoccuring[], uint8_t alarm_exception_count, uint8_t *remove_at_index);

    void default_menu(char date[11], char time[9], char day[4], char alarms[3][6], char exception_start[6], char exception_end[6], uint8_t status);
    uint8_t check_default_menu();
}
