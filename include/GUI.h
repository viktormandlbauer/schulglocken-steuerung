#include "DEFINITIONS.h"
#ifdef DEBUG_GUI
#define DEBUG
#endif

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <XPT2046_Touchscreen.h>

#define CS_PIN 4
#define TIRQ_PIN 3

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

    void update_time(bool update);
    void time(char time_string[9]);
    uint8_t check_time();
    void time_setting(char time_string[9]);
    uint8_t check_time_setting(char *time_string);

    void network_menu();
    uint8_t check_network_menu();

    void network_ntp(char *lastNtpSync, bool isEnabled);
    uint8_t check_network_ntp(bool isEnabled);

    void network_ip(uint8_t NetworkStatus, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);
    uint8_t check_network_ip();
    void network_config_update(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);

    void exception_menu();
    uint8_t check_exception_menu();

    void show_exception(char alarm_exceptions[][13], uint8_t alarm_exception_count);
    uint8_t check_show_exception();

    void add_exception(char exception_start_string[6], char exception_end_string[6]);
    uint8_t check_add_exception(char exception_start_string[6], char exception_end_string[6], bool *reoccurring);

    void weekdays_exceptions(uint8_t weekday_exception_list);
    uint8_t check_weekdays_exceptions(uint8_t *weekday_exception_list);

    void remove_exception();
    uint8_t check_remove_exception();

    void default_menu(char date[11], char time[9], char day[4], char alarms[3][6], char exception_start[6], char exception_end[6], uint8_t status);
    uint8_t check_default_menu();
}
