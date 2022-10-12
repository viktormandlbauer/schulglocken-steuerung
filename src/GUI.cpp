#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

#include <GUI.h>

Waveshare_ILI9486 Waveshield;
Adafruit_GFX &tft = Waveshield;
Adafruit_GFX_Button button;
TSPoint p;

void init_display()
{
    SPI.begin();
    Waveshield.begin();
}

bool check_button_pressed()
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

void draw_time(char *timestring)
{
    // Waveshield.setRotation(1);
    // button.initButton(&tft, X_DIM / 2, 5 * Y_DIM / 6, 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
    // button.drawButton(true);

    int size = 1;
    int y = 0;
    int x = 50;
    for (int i = 0; i < strlen(timestring); i++)
    {
        tft.drawChar(x + 30 * i, y, timestring[i], WHITE, BLACK, size);
    }
}
