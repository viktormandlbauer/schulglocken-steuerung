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

    void timeplan(char alarm_strings[][6]);
    void draw_alarm_list(char alarm_strings[][6]);
    uint8_t check_timeplan();

    void alarm_config(char *alarm_time, uint8_t *alarm_type);
    uint8_t check_alarm_config();
}
