#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

namespace GUI
{
    void init_display();
    bool display_action();
    bool check_input();
    uint8_t menu();
    uint8_t timeplan(char *time_string, char alarm_strings[][6]);
    uint8_t alarm_config(char *alarm_time, uint16_t *alarm, uint8_t *alarm_type);


}
