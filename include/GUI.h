#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

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
    void network_ntp();
    uint8_t check_network_ntp();
    void network_config(int8_t network_status, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);
    uint8_t check_network_config(int8_t *network_status, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);
    void network_config_update(uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);
}
