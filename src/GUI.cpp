#include "DEFINITIONS.h"
#ifdef DEBUG_GUI
#define DEBUG
#endif

#include <GUI.h>

Waveshare_ILI9486 Waveshield;
Adafruit_GFX &tft = Waveshield;
Adafruit_GFX_Button button;
TSPoint p;

void GUI::init_display()
{
    SPI.begin();
    Waveshield.begin();
    Waveshield.setRotation(1);
}

bool GUI::check_button_pressed()
{
    p = Waveshield.getPoint();
    Waveshield.normalizeTsPoint(p);

    if (p.z > 0)
    {
        if (button.contains(p.x, p.y))
        {
            button.press(true);
        }

        // TODO Check fuer all Buttons
    }
    if (button.isPressed())
    {
    }
}

void GUI::draw_button()
{
    Waveshield.setRotation(1);
    button.initButton(&tft, X_DIM / 2, 5 * Y_DIM / 6, 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
    button.drawButton(true);
}

void GUI::draw_warning(){
    // TODO - Show Warning on display
}

void GUI::draw_time(String timestring)
{
    uint8_t size = 5;
    uint8_t y = 0;
    uint8_t x = 50;
    for (uint8_t i = 0; i < timestring.length(); i++)
    {
        tft.drawChar(x + 30 * i, y, timestring[i], WHITE, BLACK, size);
    }
}
