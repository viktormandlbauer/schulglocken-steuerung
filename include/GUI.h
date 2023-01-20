#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>



namespace GUI
{
    void init_display();
    bool display_action();
    bool check_input();
    void draw_time(String timestring);
    uint8_t menu();
    uint8_t timeplan(char *time_string, char **alarms);
}
