#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

// Colors
#define BLACK 0x0000
#define WHITE 0xFFFF
#define BLUE 0x4C96
#define BLUE_DARK 0x02F2
#define BLUE_LIGHT 0xC6DD

// Screen dimension
#define X_DIM 480
#define Y_DIM 320

namespace GUI
{
    void init_display();
    bool check_button_pressed();
    void draw_time(String timestring);
    void draw_button();
}
